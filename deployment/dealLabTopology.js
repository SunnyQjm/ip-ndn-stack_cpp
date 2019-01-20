const fs = require('fs');
const file = "./lab_topology_raw.json";

const result = JSON.parse(fs.readFileSync(file));

let obj = {};

// 记录一下节点名和节点信息的映射关系
result.forEach(node => {
    obj[node.name] = node;
});

function findMac(myName, targetName) {
    // console.log(myName + " -> " + targetName);
    const reverseNb = obj[targetName].nbs.find((nb, index, arr) => {
        return nb.name === myName;
    });
    if(!reverseNb)
        return "";
    return obj[targetName].nics[reverseNb.nicIndex].mac;
}

result.forEach(node => {
    node.nbs.forEach(nb => {
        // console.log(nb);
        if(nb.nicIndex >= 0) {      //overlay mac
            nb.target = `ether://[${findMac(node.name, nb.name)}]`;
            nb.local = `dev://${node.nics[nb.nicIndex].name}`
        } else if(nb.nicIndex === -1) {     //overlay IP
            nb.target = `tcp://${nb.ip}`;
            nb.local = '';
        }
    })
});

console.log(JSON.stringify(result));
