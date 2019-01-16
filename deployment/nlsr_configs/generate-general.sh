#!/bin/bash
network=$1
site=$2
router=$3
cat << EOF
; the general section contains all the general settings for router
general
{
  ; mandatory configuration command section network, site and router

  network ${network}         ; name of the network the router belongs to in ndn URI format
  site ${site}    ; name of the site the router belongs to in ndn URI format
  router ${router}    ; name of the router in ndn URI format

  ; lsa-refresh-time is the time in seconds, after which router will refresh its LSAs
  lsa-refresh-time 1800      ; default value 1800. Valid values 240-7200

  ; router-dead-interval is the time in seconds after which an inactive routers
  ; LSAs are removed
  ;router-dead-interval 3600 ; default value: 2*lsa-refresh-time. Value must be larger
                             ; than lsa-refresh-time

  ; InterestLifetime (in seconds) for LSA fetching
  lsa-interest-lifetime 4    ; default value 4. Valid values 1-60

  ; sync interest lifetime of ChronoSync in milliseconds
  sync-interest-lifetime 60000  ; default value 60000. Valid values 1000-120,000

  seq-dir       /var/lib/nlsr        ; path for sequence directory (Absolute path)
}
EOF
