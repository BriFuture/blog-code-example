# -*- coding: utf-8 -*-


from .helper import createLogger
logger = createLogger('pluginLoader', stream=False, logger_prefix="[PluginLoader]")

class Events(dict):
    def __getitem__(self,name):
        if name not in self:
            super().__setitem__(name, [])
        return super().__getitem__(name)

    def trigger(self, eventName: str, eventArgs = None, **kwargs):
        """trigger event `eventName`
        """
        try:
            for e in self[eventName]:
                e(eventArgs, **kwargs)
        except Exception as exc:
            logger.warn("Exception at 'beforeRegisterRoutes: {}".format(exc))
        
    def on(self, eventName: str, callback ):
        """append callback function for certain event
        """
        self[eventName].append(callback)

import schedule

class BackTask(object):
    def __init__(self, events):
        self.events = events

    def start(self):
        """this method makes sure that endless while loop running in a subthread and will not interfere webgui
        """
        from threading import Thread
        
        mt = Thread( target= self.__loop_monitor )
        mt.daemon = True
        mt.start()
        
    def __loop_monitor(self):
        import time
        self.events.trigger("task_schedule", schedule)
        logger.info( 'start background tasks....' )
        count = 0
        while True:
            # schedule.run_pending()
            try:
                schedule.run_pending()
            except Exception as e:
                logger.warn("Schedule running: {}".format(e))
            count += 1
            if count == 3600:
                logger.debug("One Hour Pass.")
                count = 0
            time.sleep( 1 )


def load_plugins(app):
    from pathlib import Path
    import os
    pluginPath = Path(os.path.dirname(__file__)) / "plugins"
    
    import sys
    sys.path.insert(0, str(pluginPath))

    # 过滤掉非插件文件
    plugins = []
    for file in pluginPath.iterdir():
        if file.name.startswith('__'):
            continue
        if file.is_dir():
            dir_content = [x.name for x in file.iterdir()]
            if "__init__.py" in dir_content:
                plugins.append(file)
            dir_content.clear()
        elif file.is_file() and file.suffix == ".py":
            plugins.append(file)
    
    import importlib

    # 过滤掉依赖关系无法满足的插件
    logger.info("开始过滤插件。")
    plugin_module = []
    no_dependences = []
    plugin_file_names = [plugin_file.stem for plugin_file in plugins]
    for plugin_file in plugins:
        try:
            name = plugin_file.stem
            # print(' ==>', name)
            if plugin_file.is_dir():
                # following works for python3.6
                pm = importlib.import_module("{}".format(name))
                # following works for python3.7
                # pm = importlib.import_module("{}/__init__".format(name))
            else:
                pm = importlib.import_module(name)
            pm.name = name

            if(not hasattr(pm, 'requirement') or len(pm.requirement) == 0):
                logger.debug("导入无依赖插件 {}".format(pm))
                no_dependences.append(pm)
            else:
                meet = True
                for r in pm.requirement:
                    if r not in plugin_file_names:
                        meet = False
                if meet:
                    plugin_module.append(pm)
                    logger.debug("导入有依赖插件 {}".format(pm))
        except Exception as exc:
            logger.warn("Load plugin `{}` Error: {}".format(plugin_file, exc))
    plugins.clear()
    importlib.invalidate_caches()
    logger.info("过滤插件完毕，无依赖插件数：{}，有依赖插件数: {}"\
        .format(len(no_dependences), len(plugin_module)))

    num_all_plugins = len(no_dependences) + len(plugin_module)
    logger.info("准备初始化插件")
    app.m_plugins = []
    num_loaded = 0
    level_loaded = 0
    while(num_loaded < num_all_plugins and level_loaded < 3):
        num_loaded += __do_load_plugin(app, plugin_module, no_dependences)
        level_loaded += 1
    logger.info("插件初始化完成, 加载了{}个插件".format(num_loaded))

def __do_load_plugin(app, plugin_module, no_dependences):
    # 首先加载没有依赖的插件
    # logger.debug("no_dependces: {}".format(no_dependences))
    num_loaded = 0
    for nd in no_dependences:
        try:
            logger.debug("初始化插件: {}".format(nd))
            nd.init(app)
            # logger.debug("load: {}".format(nd))
            app.m_plugins.append(nd)
            num_loaded += 1
        except Exception as exc:
            logger.warn("Init plugin `{}` Error: {}".format(nd, exc))

    # logger.debug("""loaded: {}, \nno_depend: {}, 
    #     plugin_module: {}""".format(app.m_plugins, no_dependences, plugin_module))
    no_dependences.clear()

    # 满足依赖关系的插件
    for depend in plugin_module:
        for pm in app.m_plugins:
        # for req in pm.requirement:
            if pm.name in depend.requirement:
                depend.requirement.remove(pm.name)

    no_dependences.clear()
    tmp_plugins = []
    for pm in plugin_module:
        if len(pm.requirement) > 0:
            tmp_plugins.append(pm)
        else:
            no_dependences.append(pm)
    plugin_module.clear()
    for pm in tmp_plugins:
        plugin_module.append(pm)
    # logger.debug("no_depend: {}".format(no_dependences))
    return num_loaded
            
