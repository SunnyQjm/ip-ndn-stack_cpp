#!/bin/bash
cat << EOF

; the advertising section contains the configuration settings of the name prefixes
; hosted by this router

advertising
{
  ; the ndnname is used to advertised name from the router. To advertise each name prefix
  ; configure one block of ndnname configuration command for every name prefix.

  ;prefix /ndn/edu/test1           ; name in ndn URI format
  ;prefix /ndn/edu/test2
}

EOF
