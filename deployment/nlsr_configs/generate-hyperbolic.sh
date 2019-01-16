#!/bin/bash
cat << EOF

; the hyperbolic section contains the configuration settings of enabling a router to calculate
; routing table using [hyperbolic routing table calculation](http://arxiv.org/abs/0805.1266) method

hyperbolic
{
  ; commands in this section follows a strict order
  ; the switch is used to set hyperbolic routing calculation in NLSR

  state off             ; default value 'off', set value 'on' to enable hyperbolic routing table
                        ; calculation which turns link state routing 'off'. set value to 'dry-run'
                        ; to test hyperbolic routing and compare with link state routing.


  radius   123.456      ; radius of the router in hyperbolic coordinate system
  angle    1.45,2.36    ; angle of the router in hyperbolic coordinate system
}
EOF
