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
        nb.targetMac = findMac(node.name, nb.name);
        nb.localDev = node.nics[nb.nicIndex].name
    })
});

console.log(JSON.stringify(result));
