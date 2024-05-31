var fs = require("fs")

var list = JSON.parse(fs.readFileSync("info.json").toString("utf8"));

fs.writeFileSync("info__backup.json", JSON.stringify(list))

var i = 0;
while (i < 230) {
        list.nodes[0].queue.commandQueue.push({
                data: `${i}`,
                type: 2
        });

        i++;
}

fs.writeFileSync("info.json", JSON.stringify(list))
