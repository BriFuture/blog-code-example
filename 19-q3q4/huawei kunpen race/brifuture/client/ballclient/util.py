import logging


from pathlib import Path

_BASE_DIR = Path("/var")
_LOG_DIR = _BASE_DIR / "log"

_LOG_DIR.mkdir(parents=True, exist_ok=True)

## For Translating

import logging
def createLogger(name: str, stream = False, level = logging.DEBUG):
    """create logger, specify name, for example: test.log
    suffix is not necessary but helpful
    """

    # log_file = (_LOG_DIR / name)

    logger = logging.getLogger()
    # fh = logging.FileHandler(log_file.absolute())
    fh = logging.FileHandler("%s/%s" % (_LOG_DIR, name))
    fh.setFormatter(logging.Formatter("%(asctime)s %(levelname)s:  %(message)s"))
    fh.setLevel(level)
    logger.setLevel(level)
    logger.addHandler(fh)

    if stream:
        sh = logging.StreamHandler()
        sh.setLevel(level)
        sh.setFormatter(logging.Formatter("%(asctime)s %(levelname)s  %(filename)s %(lineno)s:  %(message)s"))
        logger.addHandler(sh)

    return logger

logger = None

def createShareLogger(name: str, stream = False, level = logging.DEBUG):
    # print(name)
    global logger
    logger = createLogger(name, stream=stream, level=level)