let searchAll = true;
let searchPinned = false;
let searchUploaded = false;

document.getElementById("all-button").className += " w3-grey";

function onclickAll()
{
    let allButton = document.getElementById("all-button");
    let pinnedButton = document.getElementById("pinned-button");
    let uploadedButton = document.getElementById("my-uploads-button");

    searchAll = true;
    searchPinned = false;
    searchUploaded = false;

    if (allButton.className.indexOf("w3-grey") == -1) { 
        allButton.className += " w3-grey";
        pinnedButton.className = pinnedButton.className.replace(" w3-grey", "");
        uploadedButton.className = uploadedButton.className.replace(" w3-grey", "");
    }

    search();
}

function onclickPinned()
{
    let allButton = document.getElementById("all-button");
    let pinnedButton = document.getElementById("pinned-button");
    let uploadedButton = document.getElementById("my-uploads-button");
    
    searchAll = false;
    searchPinned = true;
    searchUploaded = false;

    if (pinnedButton.className.indexOf("w3-grey") == -1) { 
        allButton.className = allButton.className.replace(" w3-grey", "");
        pinnedButton.className += " w3-grey";
        uploadedButton.className = uploadedButton.className.replace(" w3-grey", "");
    }

    search();
}

function onclickMyUploaded()
{
    let allButton = document.getElementById("all-button");
    let pinnedButton = document.getElementById("pinned-button");
    let uploadedButton = document.getElementById("my-uploads-button");
    
    searchAll = false;
    searchPinned = false;
    searchUploaded = true;

    if (uploadedButton.className.indexOf("w3-grey") == -1) { 
        allButton.className = allButton.className.replace(" w3-grey", "");
        pinnedButton.className = pinnedButton.className.replace(" w3-grey", "");
        uploadedButton.className += " w3-grey";
    }

    search();
}

function makeAllHidden()
{
    let cards = document.getElementById("mid-cards").children;
    for(let i = 0; i < cards.length; i++)
    {
        cards[i].setAttribute("hidden","");
    }
}

function makeAllShow()
{
    let cards = document.getElementById("mid-cards").children;
    for(let i = 0; i < cards.length; i++)
    {
        cards[i].removeAttribute("hidden","");
    }
}

function search()
{
    makeAllHidden();
    let tmp = document.getElementById("search-bar").children[0].value.toLowerCase();
    let rawSearch = tmp.split(" ");
    let searchWords = [];
    let searchTags = [];
    
    for(let i = 0; i < rawSearch.length; i++)
    {
        if(rawSearch[i][0]=='#')
        {
            searchTags.push(rawSearch[i].substr(1));
        }
        else
        {
            searchWords.push(rawSearch[i]);
        }
    }

    if(searchWords.length === 0) searchWords.push("");

    let cards = document.getElementById("mid-cards").children;
    for(let i = 0; i < cards.length; i++)
    {
        let textResult = false;
        let tagResult = true;
        let metaResult = false;

        let name = docs[i].name.toLowerCase();
        let desc = docs[i].description.toLowerCase();
        let text = docs[i].text.toLowerCase();

        for(let j=0;j<searchWords.length;j++)
        {
            if(name.includes(searchWords[j])) textResult = true;
            if(desc.includes(searchWords[j])) textResult = true;
            if(text.includes(searchWords[j])) textResult = true;
        }

        const tags = docs[i].tags.map(tag => tag.id.toLowerCase()); 
        for(let j = 0; j < searchTags.length; j++)
        {
            if (!tags.includes(searchTags[j])) tagResult = false;
        }

        if(searchAll === true) metaResult = true;
        if(searchPinned === true && cards[i].hasAttribute("pinned")) metaResult = true;
        if(searchUploaded === true && cards[i].hasAttribute("uploaded")) metaResult = true;

        if(textResult && tagResult && metaResult)
        {
            cards[i].removeAttribute("hidden");
        }
    }
}

function keypressSearch(event)
{
    if(event.key == "Enter")
    {
        search();
    }
}