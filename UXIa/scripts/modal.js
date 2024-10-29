function getModalDocumentCard(index)
{
    const wrapper = document.createElement("div");
    const docCard = getDocumentCard(index);

    const btnHide = document.createElement("button");
    btnHide.classList = "w3-button w3-round w3-ripple";
    btnHide.innerHTML = '<i class="fa fa-times"></i>';
    btnHide.setAttribute("onclick", `hideModal(${index})`);

    const topRight = docCard.querySelector(".w3-display-topright");
    topRight.appendChild(btnHide);

    wrapper.appendChild(docCard);

    return wrapper;
}

function openModal(index)
{
    const modal = document.getElementById("modal");
    modal.getElementsByTagName("div")[0].innerHTML = getModalDocumentCard(index).innerHTML;
    modal.style.display = "block";
}

function hideModal(index)
{
    document.getElementById("modal").style.display = "none";

    const midCards = document.getElementById("mid-cards");
    const oldCard = midCards.children[index];
    const newCard = getDocumentCard(index);
    
    midCards.replaceChild(newCard, oldCard);
}
