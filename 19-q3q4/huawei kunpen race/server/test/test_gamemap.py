import sys
from pathlib import Path

pdir = Path(".")
sys.path.insert(0, str(pdir.absolute()))
from simserver.gamemap import GameMap

gm = GameMap("map_r2m1.txt")

print(gm.teamA)
print(gm.teamB)