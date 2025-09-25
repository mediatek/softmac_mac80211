// SPDX-License-Identifier: GPL-2.0
/*
 * S1G handling
 * Copyright(c) 2020 Adapt-IP
 * Copyright (C) 2023 Intel Corporation
 */
#include <linux/ieee80211.h>
#include <net/mac80211.h>
#include "ieee80211_i.h"
#include "driver-ops.h"

void ieee80211_s1g_sta_rate_init(struct sta_info *sta)
{
	/* avoid indicating legacy bitrates for S1G STAs */
	sta->deflink.tx_stats.last_rate.flags |= IEEE80211_TX_RC_S1G_MCS;
	sta->deflink.rx_stats.last_rate =
			STA_STATS_FIELD(TYPE, STA_STATS_RATE_TYPE_S1G);
}

bool ieee80211_s1g_is_twt_setup(struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;

	if (likely(!ieee80211_is_action(mgmt->frame_control)))
		return false;

	if (likely(mgmt->u.action.category != WLAN_CATEGORY_S1G))
		return false;

	return mgmt->u.action.u.s1g.action_code == WLAN_S1G_TWT_SETUP;
}

static void
ieee80211_s1g_send_twt_setup(struct ieee80211_sub_if_data *sdata, const u8 *da,
			     const u8 *bssid, struct ieee80211_twt_setup *twt)
{
	int len = IEEE80211_MIN_ACTION_SIZE + 4 + twt->length;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_mgmt *mgmt;
	struct sk_buff *skb;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + len);
	if (!skb)
		return;

	skb_reserve(skb, local->hw.extra_tx_headroom);
	mgmt = skb_put_zero(skb, len);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_ACTION);
	memcpy(mgmt->da, da, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);

	mgmt->u.action.category = WLAN_CATEGORY_S1G;
	mgmt->u.action.u.s1g.action_code = WLAN_S1G_TWT_SETUP;
	memcpy(mgmt->u.action.u.s1g.variable, twt, 3 + twt->length);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT |
					IEEE80211_TX_INTFL_MLME_CONN_TX |
					IEEE80211_TX_CTL_REQ_TX_STATUS;
	ieee80211_tx_skb(sdata, skb);
}

static void
ieee80211_s1g_send_twt_teardown(struct ieee80211_sub_if_data *sdata,
				const u8 *da, const u8 *bssid,
				struct ieee80211_twt_teardown *td)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_mgmt *mgmt;
	struct sk_buff *skb;
	u8 *id;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom +
			    IEEE80211_MIN_ACTION_SIZE + 2);
	if (!skb)
		return;

	skb_reserve(skb, local->hw.extra_tx_headroom);
	mgmt = skb_put_zero(skb, IEEE80211_MIN_ACTION_SIZE + 2);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
					  IEEE80211_STYPE_ACTION);
	memcpy(mgmt->da, da, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);

	mgmt->u.action.category = WLAN_CATEGORY_S1G;
	mgmt->u.action.u.s1g.action_code = WLAN_S1G_TWT_TEARDOWN;
	id = (u8 *)mgmt->u.action.u.s1g.variable;
	*id = td->id;

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT |
					IEEE80211_TX_CTL_REQ_TX_STATUS;
	ieee80211_tx_skb(sdata, skb);
}

static int
ieee80211_s1g_handle_twt_demand_setup(struct ieee80211_sub_if_data *sdata,
				      struct sk_buff *skb)
{
	const size_t fixed_len = offsetof(struct ieee80211_twt_params, bcast.rtwt);
	struct ieee80211_mgmt *mgmt = (void *)skb->data;
	struct ieee80211_twt_setup *twt = (void *)mgmt->u.action.u.s1g.variable;
	struct ieee80211_twt_params *twt_agrt = (void *)twt->params;
	struct cfg80211_broadcast_twt_settings settings;
	size_t remaining_len;
	int ret;

	remaining_len = skb->len - ((u8 *)twt_agrt - skb->data);
	if (remaining_len < fixed_len)
		return -EINVAL;

	memset(&settings, 0, sizeof(settings));

	settings.min_wake_dur = twt_agrt->bcast.min_twt_dur;
	settings.wake_intv_mantissa = le16_to_cpu(twt_agrt->bcast.mantissa);
	settings.wake_intv_exp = le16_get_bits(twt_agrt->req_type,
					       IEEE80211_TWT_REQTYPE_WAKE_INT_EXP);
	settings.trigger = le16_get_bits(twt_agrt->req_type,
					 IEEE80211_TWT_REQTYPE_TRIGGER);
	settings.flow_type = le16_get_bits(twt_agrt->req_type,
					   IEEE80211_TWT_REQTYPE_FLOWTYPE);
	settings.recommendation = le16_get_bits(twt_agrt->req_type,
						IEEE80211_TWT_REQTYPE_BTWT_RECM);
	settings.setup_cmd = le16_get_bits(twt_agrt->req_type,
					   IEEE80211_TWT_REQTYPE_SETUP_CMD);

	if (le16_get_bits(twt_agrt->bcast.btwt_info,
			  IEEE80211_BTWT_RTWT_INFO_PRESENT)) {
		if (remaining_len < fixed_len + sizeof(struct ieee80211_rtwt_info))
			return -EINVAL;

		if (FIELD_GET(IEEE80211_RTWT_INFO_DL_TID_BMP_VALID,
			      twt_agrt->bcast.rtwt.info_ctrl))
			settings.dl_tid_bmp = twt_agrt->bcast.rtwt.dl_tid_bmp;

		if (FIELD_GET(IEEE80211_RTWT_INFO_UL_TID_BMP_VALID,
			      twt_agrt->bcast.rtwt.info_ctrl))
			settings.ul_tid_bmp = twt_agrt->bcast.rtwt.ul_tid_bmp;
	}

	if ((sdata->deflink.btwt_active_bitmap | BIT(0)) == ~0U)
		return -ENOSPC;

	settings.id = ffz(sdata->deflink.btwt_active_bitmap | BIT(0));
	/* FIXME: The link id shall be decided based on the effective link. */
	ret = drv_add_btwt(sdata->local, sdata, 0, &settings);
	if (ret)
		return ret;

	sdata->deflink.btwt_active_bitmap |= BIT(settings.id);
	le16p_replace_bits(&twt_agrt->bcast.btwt_info, settings.id,
			   IEEE80211_BTWT_INFO_ID);
	le16p_replace_bits(&twt_agrt->bcast.btwt_info, settings.persist,
			   IEEE80211_BTWT_INFO_PERSISTENCE);

	cfg80211_btwt_demand_sched_notify(sdata->dev, &settings, GFP_KERNEL);

	return 0;
}

static void
ieee80211_s1g_rx_twt_setup(struct ieee80211_sub_if_data *sdata,
			   struct sta_info *sta, struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (void *)skb->data;
	struct ieee80211_twt_setup *twt = (void *)mgmt->u.action.u.s1g.variable;
	struct ieee80211_twt_params *twt_agrt = (void *)twt->params;
	int ret;

	if (twt->control & IEEE80211_TWT_CONTROL_NEG_TYPE_BROADCAST &&
	    (le16_get_bits(twt_agrt->req_type, IEEE80211_TWT_REQTYPE_SETUP_CMD) ==
	     TWT_SETUP_CMD_DEMAND)) {
		ret = ieee80211_s1g_handle_twt_demand_setup(sdata, skb);
		if (ret) {
			le16p_replace_bits(&twt_agrt->req_type, TWT_SETUP_CMD_REJECT,
					   IEEE80211_TWT_REQTYPE_SETUP_CMD);
			pr_info("%s: Handle TWT demand failed: %d\n", __func__, ret);
			goto send_rsp;
		}
	}

	ret = drv_add_twt_setup(sdata->local, sdata, &sta->sta, twt);
	if (ret)
		pr_info("%s: Handle add twt setup failed: %d\n", __func__, ret);

send_rsp:
	le16p_replace_bits(&twt_agrt->req_type, TWT_RESPONDING_OR_SCHEDULING_STA,
			   IEEE80211_TWT_REQTYPE_REQUEST);
	/* TWT Information not supported yet */
	twt->control |= IEEE80211_TWT_CONTROL_RX_DISABLED;

	ieee80211_s1g_send_twt_setup(sdata, mgmt->sa, sdata->vif.addr, twt);
}

static void
ieee80211_s1g_rx_twt_teardown(struct ieee80211_sub_if_data *sdata,
			      struct sta_info *sta, struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
	struct ieee80211_twt_teardown *td = (void *)mgmt->u.action.u.s1g.variable;

	if (td->all)
		return; /* Not supported yet */

	drv_twt_teardown_request(sdata->local, sdata, &sta->sta, td);
}

static void
ieee80211_s1g_tx_twt_setup_fail(struct ieee80211_sub_if_data *sdata,
				struct sta_info *sta, struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
	struct ieee80211_twt_setup *twt = (void *)mgmt->u.action.u.s1g.variable;
	struct ieee80211_twt_params *twt_agrt = (void *)twt->params;
	struct ieee80211_twt_teardown td;

	if (twt->control & IEEE80211_TWT_CONTROL_NEG_TYPE_BROADCAST) {
		td.id = le16_get_bits(twt_agrt->bcast.btwt_info,
				      IEEE80211_BTWT_INFO_ID);
		td.neg_type = IEEE80211_TWT_NEGO_TYPE_BTWT_MBR_MGMT;
	} else {
		td.id = le16_get_bits(twt_agrt->req_type,
				      IEEE80211_TWT_REQTYPE_FLOWID);
	}

	drv_twt_teardown_request(sdata->local, sdata, &sta->sta, &td);

	ieee80211_s1g_send_twt_teardown(sdata, mgmt->sa, sdata->vif.addr, &td);
}

void ieee80211_s1g_rx_twt_action(struct ieee80211_sub_if_data *sdata,
				 struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;

	lockdep_assert_wiphy(local->hw.wiphy);

	sta = sta_info_get_bss(sdata, mgmt->sa);
	if (!sta)
		return;

	switch (mgmt->u.action.u.s1g.action_code) {
	case WLAN_S1G_TWT_SETUP:
		ieee80211_s1g_rx_twt_setup(sdata, sta, skb);
		break;
	case WLAN_S1G_TWT_TEARDOWN:
		ieee80211_s1g_rx_twt_teardown(sdata, sta, skb);
		break;
	default:
		break;
	}
}

void ieee80211_s1g_status_twt_action(struct ieee80211_sub_if_data *sdata,
				     struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;

	lockdep_assert_wiphy(local->hw.wiphy);

	sta = sta_info_get_bss(sdata, mgmt->da);
	if (!sta)
		return;

	switch (mgmt->u.action.u.s1g.action_code) {
	case WLAN_S1G_TWT_SETUP:
		/* process failed twt setup frames */
		ieee80211_s1g_tx_twt_setup_fail(sdata, sta, skb);
		break;
	default:
		break;
	}
}

void ieee80211_s1g_cap_to_sta_s1g_cap(struct ieee80211_sub_if_data *sdata,
				      const struct ieee80211_s1g_cap *s1g_cap_ie,
				      struct link_sta_info *link_sta)
{
	struct ieee80211_sta_s1g_cap *s1g_cap = &link_sta->pub->s1g_cap;

	memset(s1g_cap, 0, sizeof(*s1g_cap));

	memcpy(s1g_cap->cap, s1g_cap_ie->capab_info, sizeof(s1g_cap->cap));
	memcpy(s1g_cap->nss_mcs, s1g_cap_ie->supp_mcs_nss,
	       sizeof(s1g_cap->nss_mcs));

	s1g_cap->s1g = true;

	/* Maximum MPDU length is 1 bit for S1G */
	if (s1g_cap->cap[3] & S1G_CAP3_MAX_MPDU_LEN) {
		link_sta->pub->agg.max_amsdu_len =
			IEEE80211_MAX_MPDU_LEN_VHT_7991;
	} else {
		link_sta->pub->agg.max_amsdu_len =
			IEEE80211_MAX_MPDU_LEN_VHT_3895;
	}

	ieee80211_sta_recalc_aggregates(&link_sta->sta->sta);
}
