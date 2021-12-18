DEFINES += BUILDING_LIBCURL
DEFINES += CURL_DISABLE_LDAP
DEFINES += CURL_DISABLE_TELNET
DEFINES += CURL_DISABLE_DICT
DEFINES += CURL_DISABLE_TFTP
DEFINES += USE_SSLEA
DEFINES += USE_OPENSSL


INCLUDEPATH += $$PWD/DependLibs/libcurl/include
INCLUDEPATH += $$PWD/DependLibs/libcurl/lib


HEADERS += 	$$PWD/AppCompilerConfig.h \
    $$PWD/AppCpuArchDefines.h \
    $$PWD/AppDependLibrariesConfig.h \
    $$PWD/AppTargetOsConfig.h \
    $$PWD/DependLibs/libcurl/lib/amigaos.h \
    $$PWD/DependLibs/libcurl/lib/arpa_telnet.h \
    $$PWD/DependLibs/libcurl/lib/asyn.h \
    $$PWD/DependLibs/libcurl/lib/config-amigaos.h \
    $$PWD/DependLibs/libcurl/lib/config-android.h \
    $$PWD/DependLibs/libcurl/lib/config-dos.h \
    $$PWD/DependLibs/libcurl/lib/config-mac.h \
    $$PWD/DependLibs/libcurl/lib/config-os400.h \
    $$PWD/DependLibs/libcurl/lib/config-riscos.h \
    $$PWD/DependLibs/libcurl/lib/config-symbian.h \
    $$PWD/DependLibs/libcurl/lib/config-tpf.h \
    $$PWD/DependLibs/libcurl/lib/config-vxworks.h \
    $$PWD/DependLibs/libcurl/lib/config-win32.h \
    $$PWD/DependLibs/libcurl/lib/config-win32ce.h \
    $$PWD/DependLibs/libcurl/lib/config_libcurl.h \
    $$PWD/DependLibs/libcurl/lib/conncache.h \
    $$PWD/DependLibs/libcurl/lib/connect.h \
    $$PWD/DependLibs/libcurl/lib/content_encoding.h \
    $$PWD/DependLibs/libcurl/lib/cookie.h \
    $$PWD/DependLibs/libcurl/lib/curl_addrinfo.h \
    $$PWD/DependLibs/libcurl/lib/curl_base64.h \
    $$PWD/DependLibs/libcurl/lib/curl_ctype.h \
    $$PWD/DependLibs/libcurl/lib/curl_des.h \
    $$PWD/DependLibs/libcurl/lib/curl_endian.h \
    $$PWD/DependLibs/libcurl/lib/curl_fnmatch.h \
    $$PWD/DependLibs/libcurl/lib/curl_gethostname.h \
    $$PWD/DependLibs/libcurl/lib/curl_gssapi.h \
    $$PWD/DependLibs/libcurl/lib/curl_hmac.h \
    $$PWD/DependLibs/libcurl/lib/curl_ldap.h \
    $$PWD/DependLibs/libcurl/lib/curl_md4.h \
    $$PWD/DependLibs/libcurl/lib/curl_md5.h \
    $$PWD/DependLibs/libcurl/lib/curl_memory.h \
    $$PWD/DependLibs/libcurl/lib/curl_memrchr.h \
    $$PWD/DependLibs/libcurl/lib/curl_multibyte.h \
    $$PWD/DependLibs/libcurl/lib/curl_ntlm_core.h \
    $$PWD/DependLibs/libcurl/lib/curl_ntlm_wb.h \
    $$PWD/DependLibs/libcurl/lib/curl_path.h \
    $$PWD/DependLibs/libcurl/lib/curl_printf.h \
    $$PWD/DependLibs/libcurl/lib/curl_range.h \
    $$PWD/DependLibs/libcurl/lib/curl_rtmp.h \
    $$PWD/DependLibs/libcurl/lib/curl_sasl.h \
    $$PWD/DependLibs/libcurl/lib/curl_sec.h \
    $$PWD/DependLibs/libcurl/lib/curl_setup.h \
    $$PWD/DependLibs/libcurl/lib/curl_setup_once.h \
    $$PWD/DependLibs/libcurl/lib/curl_sha256.h \
    $$PWD/DependLibs/libcurl/lib/curl_sspi.h \
    $$PWD/DependLibs/libcurl/lib/curl_threads.h \
    $$PWD/DependLibs/libcurl/lib/curlx.h \
    $$PWD/DependLibs/libcurl/lib/dict.h \
    $$PWD/DependLibs/libcurl/lib/dotdot.h \
    $$PWD/DependLibs/libcurl/lib/easyif.h \
    $$PWD/DependLibs/libcurl/lib/escape.h \
    $$PWD/DependLibs/libcurl/lib/file.h \
    $$PWD/DependLibs/libcurl/lib/fileinfo.h \
    $$PWD/DependLibs/libcurl/lib/formdata.h \
    $$PWD/DependLibs/libcurl/lib/ftp.h \
    $$PWD/DependLibs/libcurl/lib/ftplistparser.h \
    $$PWD/DependLibs/libcurl/lib/getinfo.h \
    $$PWD/DependLibs/libcurl/lib/gopher.h \
    $$PWD/DependLibs/libcurl/lib/hash.h \
    $$PWD/DependLibs/libcurl/lib/hostcheck.h \
    $$PWD/DependLibs/libcurl/lib/hostip.h \
    $$PWD/DependLibs/libcurl/lib/http.h \
    $$PWD/DependLibs/libcurl/lib/http2.h \
    $$PWD/DependLibs/libcurl/lib/http_chunks.h \
    $$PWD/DependLibs/libcurl/lib/http_digest.h \
    $$PWD/DependLibs/libcurl/lib/http_negotiate.h \
    $$PWD/DependLibs/libcurl/lib/http_ntlm.h \
    $$PWD/DependLibs/libcurl/lib/http_proxy.h \
    $$PWD/DependLibs/libcurl/lib/if2ip.h \
    $$PWD/DependLibs/libcurl/lib/imap.h \
    $$PWD/DependLibs/libcurl/lib/inet_ntop.h \
    $$PWD/DependLibs/libcurl/lib/inet_pton.h \
    $$PWD/DependLibs/libcurl/lib/llist.h \
    $$PWD/DependLibs/libcurl/lib/memdebug.h \
    $$PWD/DependLibs/libcurl/lib/mime.h \
    $$PWD/DependLibs/libcurl/lib/multihandle.h \
    $$PWD/DependLibs/libcurl/lib/multiif.h \
    $$PWD/DependLibs/libcurl/lib/netrc.h \
    $$PWD/DependLibs/libcurl/lib/non-ascii.h \
    $$PWD/DependLibs/libcurl/lib/nonblock.h \
    $$PWD/DependLibs/libcurl/lib/parsedate.h \
    $$PWD/DependLibs/libcurl/lib/pingpong.h \
    $$PWD/DependLibs/libcurl/lib/pipeline.h \
    $$PWD/DependLibs/libcurl/lib/pop3.h \
    $$PWD/DependLibs/libcurl/lib/progress.h \
    $$PWD/DependLibs/libcurl/lib/rand.h \
    $$PWD/DependLibs/libcurl/lib/rtsp.h \
    $$PWD/DependLibs/libcurl/lib/select.h \
    $$PWD/DependLibs/libcurl/lib/sendf.h \
    $$PWD/DependLibs/libcurl/lib/setopt.h \
    $$PWD/DependLibs/libcurl/lib/setup-os400.h \
    $$PWD/DependLibs/libcurl/lib/setup-vms.h \
    $$PWD/DependLibs/libcurl/lib/share.h \
    $$PWD/DependLibs/libcurl/lib/sigpipe.h \
    $$PWD/DependLibs/libcurl/lib/slist.h \
    $$PWD/DependLibs/libcurl/lib/smb.h \
    $$PWD/DependLibs/libcurl/lib/smtp.h \
    $$PWD/DependLibs/libcurl/lib/sockaddr.h \
    $$PWD/DependLibs/libcurl/lib/socks.h \
    $$PWD/DependLibs/libcurl/lib/speedcheck.h \
    $$PWD/DependLibs/libcurl/lib/splay.h \
    $$PWD/DependLibs/libcurl/lib/ssh.h \
    $$PWD/DependLibs/libcurl/lib/strcase.h \
    $$PWD/DependLibs/libcurl/lib/strdup.h \
    $$PWD/DependLibs/libcurl/lib/strerror.h \
    $$PWD/DependLibs/libcurl/lib/strtok.h \
    $$PWD/DependLibs/libcurl/lib/strtoofft.h \
    $$PWD/DependLibs/libcurl/lib/system_win32.h \
    $$PWD/DependLibs/libcurl/lib/telnet.h \
    $$PWD/DependLibs/libcurl/lib/tftp.h \
    $$PWD/DependLibs/libcurl/lib/timeval.h \
    $$PWD/DependLibs/libcurl/lib/transfer.h \
    $$PWD/DependLibs/libcurl/lib/url.h \
    $$PWD/DependLibs/libcurl/lib/urldata.h \
    $$PWD/DependLibs/libcurl/lib/vauth/digest.h \
    $$PWD/DependLibs/libcurl/lib/vauth/ntlm.h \
    $$PWD/DependLibs/libcurl/lib/vauth/vauth.h \
    $$PWD/DependLibs/libcurl/lib/vtls/axtls.h \
    $$PWD/DependLibs/libcurl/lib/vtls/cyassl.h \
    $$PWD/DependLibs/libcurl/lib/vtls/darwinssl.h \
    $$PWD/DependLibs/libcurl/lib/vtls/gskit.h \
    $$PWD/DependLibs/libcurl/lib/vtls/gtls.h \
    $$PWD/DependLibs/libcurl/lib/vtls/mbedtls.h \
    $$PWD/DependLibs/libcurl/lib/vtls/nssg.h \
    $$PWD/DependLibs/libcurl/lib/vtls/openssl.h \
    $$PWD/DependLibs/libcurl/lib/vtls/polarssl.h \
    $$PWD/DependLibs/libcurl/lib/vtls/polarssl_threadlock.h \
    $$PWD/DependLibs/libcurl/lib/vtls/qssl.h \
    $$PWD/DependLibs/libcurl/lib/vtls/schannel.h \
    $$PWD/DependLibs/libcurl/lib/vtls/vtls.h \
    $$PWD/DependLibs/libcurl/lib/warnless.h \
    $$PWD/DependLibs/libcurl/lib/wildcard.h \
    $$PWD/DependLibs/libcurl/lib/x509asn1.h \
#    $$PWD/DependLibs/libcurl/src/tool_binmode.h \
#    $$PWD/DependLibs/libcurl/src/tool_bname.h \
#    $$PWD/DependLibs/libcurl/src/tool_cb_dbg.h \
#    $$PWD/DependLibs/libcurl/src/tool_cb_hdr.h \
#    $$PWD/DependLibs/libcurl/src/tool_cb_prg.h \
#    $$PWD/DependLibs/libcurl/src/tool_cb_rea.h \
#    $$PWD/DependLibs/libcurl/src/tool_cb_see.h \
#    $$PWD/DependLibs/libcurl/src/tool_cb_wrt.h \
#    $$PWD/DependLibs/libcurl/src/tool_cfgable.h \
#    $$PWD/DependLibs/libcurl/src/tool_convert.h \
#    $$PWD/DependLibs/libcurl/src/tool_dirhie.h \
#    $$PWD/DependLibs/libcurl/src/tool_doswin.h \
#    $$PWD/DependLibs/libcurl/src/tool_easysrc.h \
#    $$PWD/DependLibs/libcurl/src/tool_formparse.h \
#    $$PWD/DependLibs/libcurl/src/tool_getparam.h \
#    $$PWD/DependLibs/libcurl/src/tool_getpass.h \
#    $$PWD/DependLibs/libcurl/src/tool_help.h \
#    $$PWD/DependLibs/libcurl/src/tool_helpers.h \
#    $$PWD/DependLibs/libcurl/src/tool_homedir.h \
#    $$PWD/DependLibs/libcurl/src/tool_libinfo.h \
#    $$PWD/DependLibs/libcurl/src/tool_main.h \
#    $$PWD/DependLibs/libcurl/src/tool_metalink.h \
#    $$PWD/DependLibs/libcurl/src/tool_mfiles.h \
#    $$PWD/DependLibs/libcurl/src/tool_msgs.h \
#    $$PWD/DependLibs/libcurl/src/tool_operate.h \
#    $$PWD/DependLibs/libcurl/src/tool_operhlp.h \
#    $$PWD/DependLibs/libcurl/src/tool_panykey.h \
#    $$PWD/DependLibs/libcurl/src/tool_paramhlp.h \
#    $$PWD/DependLibs/libcurl/src/tool_parsecfg.h \
#    $$PWD/DependLibs/libcurl/src/tool_sdecls.h \
#    $$PWD/DependLibs/libcurl/src/tool_setopt.h \
#    $$PWD/DependLibs/libcurl/src/tool_setup.h \
#    $$PWD/DependLibs/libcurl/src/tool_sleep.h \
#    $$PWD/DependLibs/libcurl/src/tool_urlglob.h \
#    $$PWD/DependLibs/libcurl/src/tool_util.h \
#    $$PWD/DependLibs/libcurl/src/tool_version.h \
#    $$PWD/DependLibs/libcurl/src/tool_vms.h \
#    $$PWD/DependLibs/libcurl/src/tool_writeenv.h \
#    $$PWD/DependLibs/libcurl/src/tool_writeout.h \
#    $$PWD/DependLibs/libcurl/src/tool_xattr.h \
    $$PWD/DependLibs/libcurl/src/version.h \
    $$PWD/DependLibs/libcurl/lib/config-posix.h

SOURCES += 	\
    $$PWD/DependLibs/libcurl/lib/curl_addrinfo.c \
    $$PWD/DependLibs/libcurl/lib/curl_ctype.c \
    $$PWD/DependLibs/libcurl/lib/curl_des.c \
    $$PWD/DependLibs/libcurl/lib/curl_endian.c \
    $$PWD/DependLibs/libcurl/lib/curl_fnmatch.c \
    $$PWD/DependLibs/libcurl/lib/curl_gethostname.c \
    $$PWD/DependLibs/libcurl/lib/curl_gssapi.c \
    $$PWD/DependLibs/libcurl/lib/curl_memrchr.c \
    $$PWD/DependLibs/libcurl/lib/curl_multibyte.c \
    $$PWD/DependLibs/libcurl/lib/curl_ntlm_core.c \
    $$PWD/DependLibs/libcurl/lib/curl_ntlm_wb.c \
    $$PWD/DependLibs/libcurl/lib/curl_path.c \
    $$PWD/DependLibs/libcurl/lib/curl_range.c \
    $$PWD/DependLibs/libcurl/lib/curl_rtmp.c \
    $$PWD/DependLibs/libcurl/lib/curl_sasl.c \
    $$PWD/DependLibs/libcurl/lib/curl_sspi.c \
    $$PWD/DependLibs/libcurl/lib/curl_threads.c \
    $$PWD/DependLibs/libcurl/lib/ftplistparser_curl.c \
    $$PWD/DependLibs/libcurl/src/slist_wc_curl.c \
#    $$PWD/DependLibs/libcurl/src/tool_binmode.c \
#    $$PWD/DependLibs/libcurl/src/tool_bname.c \
#    $$PWD/DependLibs/libcurl/src/tool_cb_dbg.c \
#    $$PWD/DependLibs/libcurl/src/tool_cb_hdr.c \
#    $$PWD/DependLibs/libcurl/src/tool_cb_prg.c \
#    $$PWD/DependLibs/libcurl/src/tool_cb_rea.c \
#    $$PWD/DependLibs/libcurl/src/tool_cb_see.c \
#    $$PWD/DependLibs/libcurl/src/tool_cb_wrt.c \
#    $$PWD/DependLibs/libcurl/src/tool_cfgable.c \
#    $$PWD/DependLibs/libcurl/src/tool_convert.c \
#    $$PWD/DependLibs/libcurl/src/tool_dirhie.c \
#    $$PWD/DependLibs/libcurl/src/tool_doswin.c \
#    $$PWD/DependLibs/libcurl/src/tool_easysrc.c \
#    $$PWD/DependLibs/libcurl/src/tool_filetime.c \
#    $$PWD/DependLibs/libcurl/src/tool_formparse.c \
#    $$PWD/DependLibs/libcurl/src/tool_getparam.c \
#    $$PWD/DependLibs/libcurl/src/tool_getpass.c \
#    $$PWD/DependLibs/libcurl/src/tool_help.c \
#    $$PWD/DependLibs/libcurl/src/tool_helpers.c \
#    $$PWD/DependLibs/libcurl/src/tool_homedir.c \
#    $$PWD/DependLibs/libcurl/src/tool_libinfo.c \
#    $$PWD/DependLibs/libcurl/src/tool_main.c \
#    $$PWD/DependLibs/libcurl/src/tool_metalink.c \
#    $$PWD/DependLibs/libcurl/src/tool_mfiles.c \
#    $$PWD/DependLibs/libcurl/src/tool_msgs.c \
#    $$PWD/DependLibs/libcurl/src/tool_operate.c \
#    $$PWD/DependLibs/libcurl/src/tool_operhlp.c \
#    $$PWD/DependLibs/libcurl/src/tool_panykey.c \
#    $$PWD/DependLibs/libcurl/src/tool_paramhlp.c \
#    $$PWD/DependLibs/libcurl/src/tool_parsecfg.c \
#    $$PWD/DependLibs/libcurl/src/tool_setopt.c \
#    $$PWD/DependLibs/libcurl/src/tool_sleep.c \
#    $$PWD/DependLibs/libcurl/src/tool_urlglob.c \
#    $$PWD/DependLibs/libcurl/src/tool_util.c \
#    $$PWD/DependLibs/libcurl/src/tool_vms.c \
#    $$PWD/DependLibs/libcurl/src/tool_writeenv.c \
#    $$PWD/DependLibs/libcurl/src/tool_writeout.c \
#    $$PWD/DependLibs/libcurl/src/tool_xattr.c \
    $$PWD/DependLibs/libcurl/lib/url_curl.c \
    $$PWD/DependLibs/libcurl/lib/connect_curl.c \
    $$PWD/DependLibs/libcurl/lib/dict_curl.c \
    $$PWD/DependLibs/libcurl/lib/file_curl.c \
    $$PWD/DependLibs/libcurl/lib/hash_curl.c \
    $$PWD/DependLibs/libcurl/lib/getenv_curl.c \
    $$PWD/DependLibs/libcurl/lib/getinfo_curl.c \
    $$PWD/DependLibs/libcurl/lib/version_curl.c \
    $$PWD/DependLibs/libcurl/lib/ssh_curl.c \
    $$PWD/DependLibs/libcurl/lib/ftp_curl.c \
    $$PWD/DependLibs/libcurl/lib/hmac_curl.c \
    $$PWD/DependLibs/libcurl/lib/http_curl.c \
    $$PWD/DependLibs/libcurl/lib/imap_curl.c \
    $$PWD/DependLibs/libcurl/lib/md4_curl.c \
    $$PWD/DependLibs/libcurl/lib/md5_curl.c \
    $$PWD/DependLibs/libcurl/lib/rand_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/digest_curl.c \
    $$PWD/DependLibs/libcurl/lib/base64_curl.c \
    $$PWD/DependLibs/libcurl/lib/smb_curl.c \
    $$PWD/DependLibs/libcurl/lib/sha256_curl.c \
    $$PWD/DependLibs/libcurl/lib/select_curl.c \
    $$PWD/DependLibs/libcurl/lib/rtsp_curl.c \
    $$PWD/DependLibs/libcurl/lib/strcase_curl.c \
    $$PWD/DependLibs/libcurl/lib/strdup_curl.c \
    $$PWD/DependLibs/libcurl/lib/strtok_curl.c \
    $$PWD/DependLibs/libcurl/lib/telnet_curl.c \
    $$PWD/DependLibs/libcurl/lib/setopt_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/cleartext_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/vauth_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/gtls_curl.c \
    $$PWD/DependLibs/libcurl/lib/gopher_curl.c \
    $$PWD/DependLibs/libcurl/lib/x509asn1_curl.c \
    $$PWD/DependLibs/libcurl/lib/wildcard_curl.c \
    $$PWD/DependLibs/libcurl/lib/warnless_curl.c \
    $$PWD/DependLibs/libcurl/lib/transfer_curl.c \
    $$PWD/DependLibs/libcurl/lib/timeval_curl.c \
    $$PWD/DependLibs/libcurl/lib/tftp_curl.c \
    $$PWD/DependLibs/libcurl/lib/system_win32_curl.c \
    $$PWD/DependLibs/libcurl/lib/strtoofft_curl.c \
    $$PWD/DependLibs/libcurl/lib/strerror_curl.c \
    $$PWD/DependLibs/libcurl/lib/ssh-libssh_curl.c \
    $$PWD/DependLibs/libcurl/lib/splay_curl.c \
    $$PWD/DependLibs/libcurl/lib/speedcheck_curl.c \
    $$PWD/DependLibs/libcurl/lib/socks_sspi_curl.c \
    $$PWD/DependLibs/libcurl/lib/socks_gssapi_curl.c \
    $$PWD/DependLibs/libcurl/lib/socks_curl.c \
    $$PWD/DependLibs/libcurl/lib/smtp_curl.c \
    $$PWD/DependLibs/libcurl/lib/slist_curl.c \
    $$PWD/DependLibs/libcurl/lib/share_curl.c \
    $$PWD/DependLibs/libcurl/lib/sendf_curl.c \
    $$PWD/DependLibs/libcurl/lib/security_curl.c \
    $$PWD/DependLibs/libcurl/lib/progress_curl.c \
    $$PWD/DependLibs/libcurl/lib/pop3_curl.c \
    $$PWD/DependLibs/libcurl/lib/pipeline_curl.c \
    $$PWD/DependLibs/libcurl/lib/pingpong_curl.c \
    $$PWD/DependLibs/libcurl/lib/parsedate_curl.c \
    $$PWD/DependLibs/libcurl/lib/openldap_curl.c \
    $$PWD/DependLibs/libcurl/lib/nwos_curl.c \
    $$PWD/DependLibs/libcurl/lib/nwlib_curl.c \
    $$PWD/DependLibs/libcurl/lib/nonblock_curl.c \
    $$PWD/DependLibs/libcurl/lib/non-ascii_curl.c \
    $$PWD/DependLibs/libcurl/lib/netrc_curl.c \
    $$PWD/DependLibs/libcurl/lib/multi_curl.c \
    $$PWD/DependLibs/libcurl/lib/mprintf_curl.c \
    $$PWD/DependLibs/libcurl/lib/mime_curl.c \
    $$PWD/DependLibs/libcurl/lib/memdebug_curl.c \
    $$PWD/DependLibs/libcurl/lib/llist_curl.c \
    $$PWD/DependLibs/libcurl/lib/ldap_curl.c \
    $$PWD/DependLibs/libcurl/lib/krb5_curl.c \
    $$PWD/DependLibs/libcurl/lib/inet_pton_curl.c \
    $$PWD/DependLibs/libcurl/lib/inet_ntop_curl.c \
    $$PWD/DependLibs/libcurl/lib/if2ip_curl.c \
    $$PWD/DependLibs/libcurl/lib/idn_win32_curl.c \
    $$PWD/DependLibs/libcurl/lib/http_proxy_curl.c \
    $$PWD/DependLibs/libcurl/lib/http_ntlm_curl.c \
    $$PWD/DependLibs/libcurl/lib/http_negotiate_sspi_curl.c \
    $$PWD/DependLibs/libcurl/lib/http_negotiate_curl.c \
    $$PWD/DependLibs/libcurl/lib/http_digest_curl.c \
    $$PWD/DependLibs/libcurl/lib/http_chunks_curl.c \
    $$PWD/DependLibs/libcurl/lib/http2_curl.c \
    $$PWD/DependLibs/libcurl/lib/hostsyn_curl.c \
    $$PWD/DependLibs/libcurl/lib/hostip6_curl.c \
    $$PWD/DependLibs/libcurl/lib/hostip4_curl.c \
    $$PWD/DependLibs/libcurl/lib/hostip_curl.c \
    $$PWD/DependLibs/libcurl/lib/hostcheck_curl.c \
    $$PWD/DependLibs/libcurl/lib/hostasyn_curl.c \
    $$PWD/DependLibs/libcurl/lib/formdata_curl.c \
    $$PWD/DependLibs/libcurl/lib/fileinfo_curl.c \
    $$PWD/DependLibs/libcurl/lib/escape_curl.c \
    $$PWD/DependLibs/libcurl/lib/easy_curl.c \
    $$PWD/DependLibs/libcurl/lib/dotdot_curl.c \
    $$PWD/DependLibs/libcurl/lib/cookie_curl.c \
    $$PWD/DependLibs/libcurl/lib/content_encoding_curl.c \
    $$PWD/DependLibs/libcurl/lib/conncache_curl.c \
    $$PWD/DependLibs/libcurl/lib/asyn-thread_curl.c \
    $$PWD/DependLibs/libcurl/lib/asyn-ares_curl.c \
    $$PWD/DependLibs/libcurl/lib/amigaos_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/vtls_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/schannel_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/qssl_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/polarssl_threadlock_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/polarssl_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/openssl_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/nss_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/mbedtls_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/gskit_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/darwinssl_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/cyassl_curl.c \
    $$PWD/DependLibs/libcurl/lib/vtls/axtls_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/cram_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/digest_sspi_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/krb5_gssapi_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/krb5_sspi_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/ntlm_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/ntlm_sspi_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/oauth2_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/spnego_gssapi_curl.c \
    $$PWD/DependLibs/libcurl/lib/vauth/spnego_sspi_curl.c

