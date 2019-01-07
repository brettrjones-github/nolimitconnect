
INCLUDEPATH += $$PWD/../../

HEADERS += 	$$PWD/../../../../GoTvCompilerConfig.h \
	$$PWD/../../../../GoTvCpuArchDefines.h \
	$$PWD/../../../../GoTvDependLibrariesConfig.h \
	$$PWD/../../../../GoTvTargetOsConfig.h \
	$$PWD/../../config_libzip.h \
	$$PWD/../../zip.h \
	$$PWD/../../zipint.h \
	$$PWD/../../zipwin32.h

SOURCES += 	$$PWD/../../mkstemp.c \
	$$PWD/../../zip_add.c \
	$$PWD/../../zip_add_dir.c \
	$$PWD/../../zip_add_entry.c \
	$$PWD/../../zip_buffer.c \
	$$PWD/../../zip_close.c \
	$$PWD/../../zip_delete.c \
	$$PWD/../../zip_dir_add.c \
	$$PWD/../../zip_dirent.c \
	$$PWD/../../zip_discard.c \
	$$PWD/../../zip_entry.c \
	$$PWD/../../zip_err_str.c \
	$$PWD/../../zip_error.c \
	$$PWD/../../zip_error_clear.c \
	$$PWD/../../zip_error_get.c \
	$$PWD/../../zip_error_get_sys_type.c \
	$$PWD/../../zip_error_strerror.c \
	$$PWD/../../zip_error_to_str.c \
	$$PWD/../../zip_extra_field.c \
	$$PWD/../../zip_extra_field_api.c \
	$$PWD/../../zip_fclose.c \
	$$PWD/../../zip_fdopen.c \
	$$PWD/../../zip_file_add.c \
	$$PWD/../../zip_file_error_clear.c \
	$$PWD/../../zip_file_error_get.c \
	$$PWD/../../zip_file_get_comment.c \
	$$PWD/../../zip_file_get_external_attributes.c \
	$$PWD/../../zip_file_get_offset.c \
	$$PWD/../../zip_file_rename.c \
	$$PWD/../../zip_file_replace.c \
	$$PWD/../../zip_file_set_comment.c \
	$$PWD/../../zip_file_set_external_attributes.c \
	$$PWD/../../zip_file_set_mtime.c \
	$$PWD/../../zip_file_strerror.c \
	$$PWD/../../zip_filerange_crc.c \
	$$PWD/../../zip_fopen.c \
	$$PWD/../../zip_fopen_encrypted.c \
	$$PWD/../../zip_fopen_index.c \
	$$PWD/../../zip_fopen_index_encrypted.c \
	$$PWD/../../zip_fread.c \
	$$PWD/../../zip_get_archive_comment.c \
	$$PWD/../../zip_get_archive_flag.c \
	$$PWD/../../zip_get_compression_implementation.c \
	$$PWD/../../zip_get_encryption_implementation.c \
	$$PWD/../../zip_get_file_comment.c \
	$$PWD/../../zip_get_name.c \
	$$PWD/../../zip_get_num_entries.c \
	$$PWD/../../zip_get_num_files.c \
	$$PWD/../../zip_hash.c \
	$$PWD/../../zip_io_util.c \
	$$PWD/../../zip_memdup.c \
	$$PWD/../../zip_name_locate.c \
	$$PWD/../../zip_new.c \
	$$PWD/../../zip_open.c \
	$$PWD/../../zip_rename.c \
	$$PWD/../../zip_replace.c \
	$$PWD/../../zip_set_archive_comment.c \
	$$PWD/../../zip_set_archive_flag.c \
	$$PWD/../../zip_set_default_password.c \
	$$PWD/../../zip_set_file_comment.c \
	$$PWD/../../zip_set_file_compression.c \
	$$PWD/../../zip_set_name.c \
	$$PWD/../../zip_source_begin_write.c \
	$$PWD/../../zip_source_buffer.c \
	$$PWD/../../zip_source_call.c \
	$$PWD/../../zip_source_close.c \
	$$PWD/../../zip_source_commit_write.c \
	$$PWD/../../zip_source_crc.c \
	$$PWD/../../zip_source_deflate.c \
	$$PWD/../../zip_source_error.c \
	$$PWD/../../zip_source_file.c \
	$$PWD/../../zip_source_filep.c \
	$$PWD/../../zip_source_free.c \
	$$PWD/../../zip_source_function.c \
	$$PWD/../../zip_source_is_deleted.c \
	$$PWD/../../zip_source_layered.c \
	$$PWD/../../zip_source_open.c \
	$$PWD/../../zip_source_pkware.c \
	$$PWD/../../zip_source_read.c \
	$$PWD/../../zip_source_remove.c \
	$$PWD/../../zip_source_rollback_write.c \
	$$PWD/../../zip_source_seek.c \
	$$PWD/../../zip_source_seek_write.c \
	$$PWD/../../zip_source_stat.c \
	$$PWD/../../zip_source_supports.c \
	$$PWD/../../zip_source_tell.c \
	$$PWD/../../zip_source_tell_write.c \
	$$PWD/../../zip_source_win32a.c \
	$$PWD/../../zip_source_win32handle.c \
	$$PWD/../../zip_source_win32utf8.c \
	$$PWD/../../zip_source_win32w.c \
	$$PWD/../../zip_source_window.c \
	$$PWD/../../zip_source_write.c \
	$$PWD/../../zip_source_zip.c \
	$$PWD/../../zip_source_zip_new.c \
	$$PWD/../../zip_stat.c \
	$$PWD/../../zip_stat_index.c \
	$$PWD/../../zip_stat_init.c \
	$$PWD/../../zip_strerror.c \
	$$PWD/../../zip_string.c \
	$$PWD/../../zip_unchange.c \
	$$PWD/../../zip_unchange_all.c \
	$$PWD/../../zip_unchange_archive.c \
	$$PWD/../../zip_unchange_data.c \
	$$PWD/../../zip_utf-8.c

