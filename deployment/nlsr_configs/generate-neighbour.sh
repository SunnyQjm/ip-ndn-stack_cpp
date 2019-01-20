#!/bin/bash
name=$1
path=$2

TEMP_DIR=${path}/temp
transName=$(echo ${name//\//.})
transName=$(echo ${transName#.})
nbs=$(cat ${TEMP_DIR}/${transName}.nbs)
cat << EOF
neighbors
{
  ; in case hello interest timed out, router will try 'hello-retries' times at 'hello-timeout'
  ; seconds interval before giving up for any neighbors (deciding link is down)

   hello-retries 3                     ; interest retries number in integer. Default value 3
                                       ; valid values 1-10

   hello-timeout 1                    ; interest time out value in integer. Default value 1
                                       ; Valid values 1-15

   hello-interval  60                  ; interest sending interval in seconds. Default value 60
                                       ; valid values 30-90

  ; adj-lsa-build-interval is the time to wait in seconds after an Adjacency LSA build is scheduled
  ; before actually building the Adjacency LSA

  adj-lsa-build-interval 5   ; default value 5. Valid values 0-5. It is recommended that
                             ; adj-lsa-build-interval have a lower value than routing-calc-interval

  ; first-hello-interval is the time to wait in seconds before sending the first Hello Interest

  first-hello-interval  10   ; Default value 10. Valid values 0-10

  face-dataset-fetch-tries 3 ; default is 3. Valid values 1-10. The FaceDataset is
                             ; gotten from NFD, and is needed to configure NLSR
                             ; correctly. It is recommended not to set this
                             ; variable too high, because it could cause
                             ; congestion for NFD.

  face-dataset-fetch-interval 3600 ; default is 3600. Valid values 1800-5400.
                                   ; This controls how often (in seconds) NLSR will attempt to
                                   ; fetch a FaceStatus dataset from NFD.

  ; neighbor command is used to configure router's neighbor. Each neighbor will need
  ; one block of neighbor command
EOF

length=`echo ${nbs} | jq '.|length'`
let 'length = length - 1'

for index in `seq 0 ${length}`
do
    neighbourName=$(echo ${nbs} | jq -r ".[$index].name")
    faceUri=$(echo ${nbs} | jq -r ".[$index].target")
    linkCost=25
    cat << EOF
    neighbor 
    {
        name ${neighbourName} ; name prefix of the neighbor router consists
                            ; of network, site-name and router-name
        face-uri ${faceUri}   ; face uri of the face connected to the neighbor
        link-cost ${linkCost} ; cost of the connecting link to neighbor

    }
EOF
done

echo }
