#!/bin/zsh
# Genereert een Home Assistant klok-pagina uit HA-clock-view-template.yaml.
#
# Gebruik:  ./make-clock-view.sh <device-id> [Weergavenaam]
# Voorbeeld: ./make-clock-view.sh espredpcb "ESPredPCB"
#            ./make-clock-view.sh match_klok "Match klok"
#
# De device-id is de BLE-naam van de klok in kleine letters, spaties vervangen
# door _ (zo maakt Home Assistant de entity-id's aan). De uitvoer plak je in het
# dashboard (raw-configuratie-editor) onder "views:", op hetzelfde
# inspring-niveau als de andere pagina's.

if [ -z "$1" ]; then
  echo "Gebruik: $0 <device-id> [Weergavenaam]" >&2
  exit 1
fi

ID="$1"
NAME="${2:-$1}"
DIR="$(dirname "$0")"

sed -e "s/CLOCKID/${ID}/g" -e "s/CLOCKNAME/${NAME}/g" "$DIR/HA-clock-view-template.yaml" | grep -v '^#'
