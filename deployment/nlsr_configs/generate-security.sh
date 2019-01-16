#!/bin/bash
cat << EOF

security
{
  validator
  {
    trust-anchor
    {
      type any
    }
  }
  prefix-update-validator
  {
    trust-anchor
    {
      type any
    }
  }
}
EOF
