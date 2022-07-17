TEMPLATE = subdirs

qtHaveModule(widgets): SUBDIRS += GoTvPlayer


qtHaveModule(opengl):!qtConfig(opengles2): SUBDIRS += opengl
