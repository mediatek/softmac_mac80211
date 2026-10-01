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
#include <crypto/backport-aes-cbc-macs.h>
#endif /* CPTCFG_BPAUTO_BUILD_CRYPTO_LIB_AES_CBC_MACS */
