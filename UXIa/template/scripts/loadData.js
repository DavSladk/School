var tags;
var docs;
var myUrl = "https://www.stud.fit.vutbr.cz/~xsladk07/data/";

async function getData(url) {
    let obj = await (await fetch(url)).json();
    return obj;
}

(async () => {
  tags = await getData(myUrl+'tags.json')
  console.log(tags)
})();

(async () => {
    docs = await getData(myUrl+'docs.json')
    console.log(docs)
    // TO DO populate docs
})();
