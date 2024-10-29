let left_cards = [];
let left_cards_index = [];

function sortDates()
{
    for(let i = 0; i < docs.length; i++)
    {
        console.log(docs.length);
        if(left_cards.length == 0)
        {
            left_cards.push(docs[i]);  
            left_cards_index.push(i);  
        }
        else
        {
            let found = 0;
            for(let j = 0; j < left_cards.length; j++)
            {
                if(left_cards[j].uploadDate < docs[i].uploadDate)
                {
                    left_cards.splice(j, 0, docs[i]);
                    left_cards_index.splice(j, 0, i);
                    found = 1;
                    break;
                }
            }
            if(found == 0)
            {
                left_cards.push(docs[i]);
                left_cards_index.push(i);
            }
        }
    } 
}

function populateLeftCards()
{
    let leftCardsContainer = document.getElementById("latest-uploads-cards");
    let midparent = document.getElementById("mid-cards");
    sortDates();
    console.log(left_cards);
    for(let i = 0; i < 5; i++)
    {
        let index = left_cards_index[i]
        const docData = docs[index];
    
        let cardRoot = document.createElement("div");
        cardRoot.className = "w3-card w3-padding-small w3-margin-bottom w3-border w3-hover-border-teal w3-round";
        cardRoot.setAttribute("index", index);
        cardRoot.setAttribute("onclick", `openModal(${index})`);
        cardRoot.id = `latest-uploads-${index}`;


        let title = document.createElement("b");
        title.className = "w3-large";
        title.innerHTML = docData.name;
        
        let tags = document.createElement("div");
        tags.className = "w3-bar";
        
        docData.tags.forEach(tag => tags.appendChild(getTag(tag, docData.tags)));

        let description = document.createElement("p");
        description.className = "w3-medium";
        description.innerHTML = docData.description;

        let infoRoot = document.createElement("div");
        infoRoot.className = "w3-small w3-text-grey";

        let upload = document.createElement("div");
        upload.className = "w3-bar-item";
        upload.innerHTML = "Upload date: "+ docs[index].uploadDate;

        infoRoot.appendChild(upload);
        
        cardRoot.appendChild(title);
        cardRoot.appendChild(tags);
        cardRoot.appendChild(description);
        cardRoot.appendChild(infoRoot);
        
        leftCardsContainer.appendChild(cardRoot);
    }
}