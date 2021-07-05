cmake_minimum_required(VERSION 3.12.2)

project(libssh)

set(CMAKE_CONFIGURATION_TYPES "Debug"
    CACHE STRING "Configuration types" FORCE)

add_library(libssh STATIC
  ../../agent.h
  ../../agent_ssh.c
  ../../auth.h
  ../../auth_ssh.c
  ../../auth1_ssh.c
  ../../base64_ssh.c
  ../../bignum.h
  ../../bignum_ssh.c
  ../../bind.h
  ../../bind_ssh.c
  ../../blf.h
  ../../buffer.h
  ../../buffer_ssh.c
  ../../callbacks.h
  ../../callbacks_ssh.c
  ../../channels.h
  ../../channels_ssh.c
  ../../channels1_ssh.c
  ../../client_ssh.c
  ../../config_ssh.c
  ../../connect_ssh.c
  ../../crc32.h
  ../../crc32_ssh.c
  ../../crypto.h
  ../../curve25519.h
  ../../curve25519_ssh.c
  ../../dh.h
  ../../dh_ssh.c
  ../../ecdh.h
  ../../ecdh_ssh.c
  ../../ed25519.h
  ../../error_ssh.c
  ../../external/bcrypt_pbkdf_ssh.c
  ../../external/blowfish_ssh.c
  ../../external/curve25519_ref_ssh.c
  ../../external/ed25519_ssh.c
  ../../external/fe25519_ssh.c
  ../../external/ge25519_ssh.c
  ../../external/sc25519_ssh.c
  ../../fe25519.h
  ../../gcrypt_missing_ssh.c
  ../../ge25519.h
  ../../getpass_ssh.c
  ../../gssapi.h
  ../../gssapi_ssh.c
  ../../gzip_ssh.c
  ../../init_ssh.c
  ../../kex.h
  ../../kex_ssh.c
  ../../kex1_ssh.c
  ../../keys.h
  ../../known_hosts_ssh.c
  ../../knownhosts.h
  ../../legacy.h
  ../../legacy_ssh.c
  ../../libcrypto.h
  ../../libcrypto_ssh.c
  ../../libgcrypt.h
  ../../libgcrypt_ssh.c
  ../../libssh.h
  ../../libssh_priv.h
  ../../libssh_string.h
  ../../libssh_string_ssh.c
  ../../libsshpp.hpp
  ../../log_ssh.c
  ../../match_ssh.c
  ../../messages.h
  ../../messages_ssh.c
  ../../misc.h
  ../../misc_ssh.c
  ../../options.h
  ../../options_ssh.c
  ../../packet.h
  ../../packet_cb_ssh.c
  ../../packet_crypt_ssh.c
  ../../packet_ssh.c
  ../../packet1_ssh.c
  ../../pcap.h
  ../../pcap_ssh.c
  ../../pki.h
  ../../pki_container_openssh_ssh.c
  ../../pki_crypto_ssh.c
  ../../pki_ed25519_ssh.c
  ../../pki_gcrypt_ssh.c
  ../../pki_priv.h
  ../../pki_ssh.c
  ../../poll.h
  ../../poll_ssh.c
  ../../sc25519.h
  ../../scp.h
  ../../scp_ssh.c
  ../../server.h
  ../../server_ssh.c
  ../../session.h
  ../../session_ssh.c
  ../../sftp.h
  ../../sftp_ssh.c
  ../../sftpserver_ssh.c
  ../../socket.h
  ../../socket_ssh.c
  ../../ssh1.h
  ../../ssh2.h
  ../../threads.h
  ../../threads/pthread_ssh.c
  ../../threads_ssh.c
  ../../wrapper.h
  ../../wrapper_ssh.c
)
# Output file name
set_target_properties(libssh
  PROPERTIES
  OUTPUT_NAME_DEBUG libsshD
)

# Additional include directories
set_property(TARGET libssh
  APPEND PROPERTY INCLUDE_DIRECTORIES
  $<$<CONFIG:Debug>:
    F:\NoLimitConnectWindows\DependLibs\libssh\build\VS2017\..\..\;
    F:\NoLimitConnectWindows\DependLibs\libssh\build\VS2017\..\..\..\;
    F:\NoLimitConnectWindows\DependLibs\libssh\build\VS2017\..\..\..\openssl-1.0.2p\include;
    F:\NoLimitConnectWindows\DependLibs\libssh\build\VS2017\..\..\..\..\;
    F:\NoLimitConnectWindows\DependLibs\libssh\build\VS2017\..\..\..\..\sysheaders\>
)

# Preprocessor definitions
target_compile_definitions(libssh PRIVATE
  $<$<CONFIG:Debug>:_MBCS;TARGET_OS_WINDOWS;LIB_STATIC;LIBSSH_STATIC;_X86_;_DEBUG;WIN32;HAVE_CONFIG_H;_LIB;__STDC_CONSTANT_MACROS;_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1;_CRT_SECURE_NO_WARNINGS;_CRT_NONSTDC_NO_DEPRECATE>
)

# Minimal rebuild
if (MSVC)
  target_compile_options(libssh PRIVATE
    "$<$<CONFIG:Debug>:/Gm->"
  )
endif ()

# Multi-processor compilation
if (MSVC)
  target_compile_options(libssh PRIVATE
    "$<$<CONFIG:Debug>:/MP>"
  )
endif ()

# Precompiled header files
if (MSVC)
  target_compile_options(libssh PRIVATE
    ""
  )
endif ()
