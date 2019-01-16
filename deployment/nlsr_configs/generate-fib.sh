#!/bin/bash
cat << EOF

; the fib section is used to configure fib entrys type to ndn FIB updated by NLSR

fib
{
  ; the max-faces-per-prefix is used to limit the number of faces for each name prefixes
  ; by NLSR in ndn FIB

  max-faces-per-prefix 3   ; default value 0. Valid value 0-60. By default (value 0) NLSR adds
                           ; all available faces for each reachable name prefixes in NDN FIB

  ; routing-calc-interval is the time to wait in seconds after a routing table calculation is
  ; scheduled before actually performing the routing table calculation

  routing-calc-interval 15   ; default value 15. Valid values 0-15. It is recommended that
                             ; routing-calc-interval have a higher value than adj-lsa-build-interval
}


EOF
