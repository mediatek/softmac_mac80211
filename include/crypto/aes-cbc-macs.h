/* Automatically created during backport process */
#ifndef CPTCFG_BPAUTO_BUILD_CRYPTO_LIB_AES_CBC_MACS
#include_next <crypto/aes-cbc-macs.h>
#else
#undef crypto_aes_sbox
#define crypto_aes_sbox LINUX_BACKPORT(crypto_aes_sbox)
#undef crypto_aes_inv_sbox
#define crypto_aes_inv_sbox LINUX_BACKPORT(crypto_aes_inv_sbox)
#undef aes_enc_tab
#define aes_enc_tab LINUX_BACKPORT(aes_enc_tab)
#undef aes_dec_tab
#define aes_dec_tab LINUX_BACKPORT(aes_dec_tab)
#undef aes_expandkey
#define aes_expandkey LINUX_BACKPORT(aes_expandkey)
#undef aes_preparekey
#define aes_preparekey LINUX_BACKPORT(aes_preparekey)
#undef aes_prepareenckey
#define aes_prepareenckey LINUX_BACKPORT(aes_prepareenckey)
#undef aes_encrypt
#define aes_encrypt LINUX_BACKPORT(aes_encrypt)
#undef aes_decrypt
#define aes_decrypt LINUX_BACKPORT(aes_decrypt)
#undef aes_cmac_preparekey
#define aes_cmac_preparekey LINUX_BACKPORT(aes_cmac_preparekey)
#undef aes_xcbcmac_preparekey
#define aes_xcbcmac_preparekey LINUX_BACKPORT(aes_xcbcmac_preparekey)
#undef aes_cmac_update
#define aes_cmac_update LINUX_BACKPORT(aes_cmac_update)
#undef aes_cmac_final
#define aes_cmac_final LINUX_BACKPORT(aes_cmac_final)
#undef aes_ecb_encrypt
#define aes_ecb_encrypt LINUX_BACKPORT(aes_ecb_encrypt)
#undef aes_ecb_decrypt
#define aes_ecb_decrypt LINUX_BACKPORT(aes_ecb_decrypt)
#undef aes_cbc_encrypt
#define aes_cbc_encrypt LINUX_BACKPORT(aes_cbc_encrypt)
#undef aes_cbc_decrypt
#define aes_cbc_decrypt LINUX_BACKPORT(aes_cbc_decrypt)
#undef aes_cbc_cts_encrypt
#define aes_cbc_cts_encrypt LINUX_BACKPORT(aes_cbc_cts_encrypt)
#undef aes_cbc_cts_decrypt
#define aes_cbc_cts_decrypt LINUX_BACKPORT(aes_cbc_cts_decrypt)
#undef aes_ctr
#define aes_ctr LINUX_BACKPORT(aes_ctr)
#undef aes_xctr
#define aes_xctr LINUX_BACKPORT(aes_xctr)
#undef aes_xts_preparekey
#define aes_xts_preparekey LINUX_BACKPORT(aes_xts_preparekey)
#undef aes_xts_encrypt
#define aes_xts_encrypt LINUX_BACKPORT(aes_xts_encrypt)
#undef aes_xts_decrypt
#define aes_xts_decrypt LINUX_BACKPORT(aes_xts_decrypt)
#undef aes_gcm_preparekey
#define aes_gcm_preparekey LINUX_BACKPORT(aes_gcm_preparekey)
#undef aes_gcm_init
#define aes_gcm_init LINUX_BACKPORT(aes_gcm_init)
#undef aes_gcm_auth_update
#define aes_gcm_auth_update LINUX_BACKPORT(aes_gcm_auth_update)
#undef aes_gcm_encrypt_update
#define aes_gcm_encrypt_update LINUX_BACKPORT(aes_gcm_encrypt_update)
#undef aes_gcm_decrypt_update
#define aes_gcm_decrypt_update LINUX_BACKPORT(aes_gcm_decrypt_update)
#undef aes_gcm_encrypt_final
#define aes_gcm_encrypt_final LINUX_BACKPORT(aes_gcm_encrypt_final)
#undef aes_gcm_decrypt_final
#define aes_gcm_decrypt_final LINUX_BACKPORT(aes_gcm_decrypt_final)
#undef aes_gcm_encrypt
#define aes_gcm_encrypt LINUX_BACKPORT(aes_gcm_encrypt)
#undef aes_gcm_decrypt
#define aes_gcm_decrypt LINUX_BACKPORT(aes_gcm_decrypt)
#undef aes_ccm_preparekey
#define aes_ccm_preparekey LINUX_BACKPORT(aes_ccm_preparekey)
#undef aes_ccm_init
#define aes_ccm_init LINUX_BACKPORT(aes_ccm_init)
#undef aes_ccm_auth_update
#define aes_ccm_auth_update LINUX_BACKPORT(aes_ccm_auth_update)
#undef aes_ccm_encrypt_update
#define aes_ccm_encrypt_update LINUX_BACKPORT(aes_ccm_encrypt_update)
#undef aes_ccm_decrypt_update
#define aes_ccm_decrypt_update LINUX_BACKPORT(aes_ccm_decrypt_update)
#undef aes_ccm_encrypt_final
#define aes_ccm_encrypt_final LINUX_BACKPORT(aes_ccm_encrypt_final)
#undef aes_ccm_decrypt_final
#define aes_ccm_decrypt_final LINUX_BACKPORT(aes_ccm_decrypt_final)
#undef aes_ccm_encrypt
#define aes_ccm_encrypt LINUX_BACKPORT(aes_ccm_encrypt)
#undef aes_ccm_decrypt
#define aes_ccm_decrypt LINUX_BACKPORT(aes_ccm_decrypt)
#include <crypto/backport-aes-cbc-macs.h>
#endif /* CPTCFG_BPAUTO_BUILD_CRYPTO_LIB_AES_CBC_MACS */
