import logging


from pathlib import Path

_BASE_DIR = Path("./tmp/clients")
_LOG_DIR = _BASE_DIR / "logs"
_CONFIG_DIR = _BASE_DIR / "configs"

_LOG_DIR.mkdir(parents=True, exist_ok=True)
_CONFIG_DIR.mkdir(parents=True, exist_ok=True)

## For Translating

LANGUAGE_DIR = (Path(__file__).parent / "locale").resolve()
import gettext
def initGetText(domain="myfacilities") -> gettext.gettext:
    gettext.bindtextdomain(domain, LANGUAGE_DIR)
    gettext.textdomain(domain)
    gettext.find(domain, "locale", languages=["zh_CN", "en_US"])
    return gettext.gettext

import logging
def createLogger(name: str, stream = False, level = logging.DEBUG):
    """create logger, specify name, for example: test.log
    suffix is not necessary but helpful
    """

    log_file = _LOG_DIR / name

    logger = logging.getLogger()
    fh = logging.FileHandler(log_file)
    fh.setFormatter(logging.Formatter("%(asctime)s %(levelname)s %(filename)s %(lineno)s:  %(message)s"))
    fh.setLevel(level)
    logger.setLevel(level)
    logger.addHandler(fh)

    if stream:
        sh = logging.StreamHandler()
        sh.setLevel(level)
        sh.setFormatter(logging.Formatter("%(asctime)s %(levelname)s %(filename)s %(lineno)s:  %(message)s"))
        logger.addHandler(sh)

    return logger

logger = createLogger("server.log", stream=True)