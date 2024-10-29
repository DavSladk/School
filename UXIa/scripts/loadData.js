var tags;
var docs;
//var myUrl = "https://www.stud.fit.vutbr.cz/~xsladk07/data/";
var myUrl = "data/";

async function getData(url) {
    let obj = await (await fetch(url)).json();
    return obj;
}

(async () => {
    tags = await getData(myUrl + 'tags.json');
    docs = await getData(myUrl + 'docs.json');

    docs.forEach(doc => {
        doc.tags.sort((a, b) => a.score < b.score ? 1 : -1);
    });

    console.log(tags);
    console.log(docs);
    populateMidCards();
    populateLeftCards();
})();
