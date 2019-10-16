
def startServer():
    from .server import SimServer
    import argparse
    # import sys

    ap = argparse.ArgumentParser("")
    ap.add_argument("-H")
    ap.add_argument("-P", required=True, help="port")
    ap.add_argument("-M", required=True, help="map")

    args = vars(ap.parse_args())

    ss = SimServer(int(args["P"]), args["M"])

    ss.start()