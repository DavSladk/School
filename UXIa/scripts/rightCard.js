const RECENT_MAX_COUNT = 5;
var recent = [];

function queueRecent(el)
{
    const cardRootIndex = el.parentElement.parentElement.getAttribute("index");
    if (cardRootIndex == null)
    {
        console.error("Cannot find parent card index");
        return;
    }

    // If the given card is already displayed in recent cards
    if (recent.includes(cardRootIndex))
    {
        if (recent.length == 1)
        {
            return;
        }
        shiftToFirstPlace(cardRootIndex);
        return;
    }
    
    if (docs[cardRootIndex] == undefined)
    {
        console.error(`Document with index ${cardRootIndex} doesn't exist`);
        return;
    }

    // If the recent cards cap will be exceeded and the last card will be removed
    if (recent.length == RECENT_MAX_COUNT)
    {
        const poppedIndex = recent.shift();
        removeCard(poppedIndex);
    }
    recent.push(cardRootIndex);

    createCard(cardRootIndex);
}

function shiftToFirstPlace(index)
{
    const card = document.getElementById(`recent-card-${index}`);
    if (card == null)
    {
        console.error(`Cannot find a recent card with given index (${index})`);
        return;
    }

    let recentCardsContainer = document.getElementById("recently-viewed-cards");
    if (recentCardsContainer == null)
    {
        console.error("Cannot find recent cards container by its ID (recently-viewed-card)");
        return;
    }

    recentCardsContainer.prepend(card);

    const barrier = recent.findIndex(element => element === index);
    for (let i = barrier; i < recent.length - 1; i++)
    {
        recent[i] = recent[i + 1];
    }
    recent[recent.length - 1] = index;
}

function removeCard(index)
{
    const card = document.getElementById(`recent-card-${index}`);
    if (card == null)
    {
        console.error(`Cannot find a recent card with given index (${index})`);
        return;
    }

    card.remove();
}

function createCard(index)
{
    let recentCardsContainer = document.getElementById("recently-viewed-cards");
    if (recentCardsContainer == null)
    {
        console.error("Cannot find recent cards container by its ID (recently-viewed-card)");
        return;
    }
        
    const docData = docs[index];
    
    let cardRoot = document.createElement("div");
    cardRoot.className = "w3-card w3-padding-small w3-margin-bottom w3-border w3-hover-border-teal w3-round";
    cardRoot.setAttribute("index", index);
    cardRoot.setAttribute("onclick", `openModal(${index})`);
    cardRoot.id = `recent-card-${index}`;

    let title = document.createElement("b");
    title.className = "w3-large";
    title.innerHTML = docData.name;

    let tags = document.createElement("div");
    tags.className = "w3-bar";
    
    docData.tags.forEach(tag => tags.appendChild(getTag(tag, docData.tags)));

    let description = document.createElement("p");
    description.className = "w3-medium";
    description.innerHTML = docData.description;
    
    cardRoot.appendChild(title);
    cardRoot.appendChild(tags);
    cardRoot.appendChild(description);
    
    recentCardsContainer.insertBefore(cardRoot, recentCardsContainer.firstChild);
}