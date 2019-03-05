# -*- coding: utf-8 -*-

from pathlib import Path

_BASE_DIR = Path.home() / ".shells"
_LOG_DIR = _BASE_DIR / "logs"
_CONFIG_DIR = _BASE_DIR / "configs"

_LOG_DIR.mkdir(parents=True, exist_ok=True)
_CONFIG_DIR.mkdir(parents=True, exist_ok=True)

def createLogger(name: str, stream = False):
    """create logger, specify name, for example: test.log
    suffix is not necessary but helpful
    """
    import logging

    log_file = _LOG_DIR / name

    logger = logging.getLogger()
    fh = logging.FileHandler(log_file)
    fh.setFormatter(logging.Formatter("%(asctime)s %(levelname)s:  %(message)s"))
    fh.setLevel(logging.DEBUG)
    logger.setLevel(logging.DEBUG)
    logger.addHandler(fh)

    if stream:
        sh = logging.StreamHandler()
        sh.setLevel(logging.DEBUG)
        sh.setFormatter(logging.Formatter("%(asctime)s %(levelname)s:  %(message)s"))
        logger.addHandler(sh)

    return logger