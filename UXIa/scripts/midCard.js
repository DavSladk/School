function onclickUp(el)
{
    if (el.className.indexOf("w3-text-green") == -1) {
        el.className = el.className.replace(" w3-text-white", " w3-text-green");
        el.className = el.className.replace(" w3-green", "");
        let at = Number(el.getAttribute("votes"))-1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-up"></i> ' + at;
    } else {
        el.className = el.className.replace(" w3-text-green", " w3-text-white");
        el.className += " w3-green";
        let at = Number(el.getAttribute("votes"))+1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-up"></i> ' + at;
    }

    let red = el.parentElement.getElementsByTagName("button")[1];
    if (red.className.indexOf("w3-text-red") == -1) {
        red.className = red.className.replace(" w3-text-white", " w3-text-red");
        red.className = red.className.replace(" w3-red", "");
        let at = Number(red.getAttribute("votes"))-1;
        red.setAttribute("votes", at);
        red.innerHTML = '<i class="fa fa-arrow-down"></i> ' + at;
    }
}

function onclickDown(el)
{
    if (el.className.indexOf("w3-text-red") == -1) {
        el.className = el.className.replace(" w3-text-white", " w3-text-red");
        el.className = el.className.replace(" w3-red", "");
        let at = Number(el.getAttribute("votes"))-1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-down"></i> ' + at;
    } else {
        el.className = el.className.replace(" w3-text-red", " w3-text-white");
        el.className += " w3-red";
        let at = Number(el.getAttribute("votes"))+1;
        el.setAttribute("votes", at);
        el.innerHTML = '<i class="fa fa-arrow-down"></i> ' + at;
    }

    let green = el.parentElement.getElementsByTagName("button")[0];
    if (green.className.indexOf("w3-text-green") == -1) {
        green.className = green.className.replace(" w3-text-white", " w3-text-green");
        green.className = green.className.replace(" w3-green", "");
        let at = Number(green.getAttribute("votes"))-1;
        green.setAttribute("votes", at);
        green.innerHTML = '<i class="fa fa-arrow-up"></i> ' + at;
    }
}

function onclickOpen(el)
{
    queueRecent(el);
}

function onclickPin(el, index)
{
    const doc = docs[index];
    if (el.className.indexOf("w3-grey") == -1) {
        doc.pinned = true;
        el.className += " w3-grey";
        el.parentElement.parentElement.setAttribute("pinned","");
    } else {
        doc.pinned = false;
        el.className = el.className.replace(" w3-grey", "");
        el.parentElement.parentElement.removeAttribute("pinned");
    }
}

function populateMidCards()
{
    const parent = document.getElementById("mid-cards");

    for (let i = 0; i < docs.length; i++)
    {
        // ROOT
        let root = document.createElement("div");
        root.className = "w3-container w3-margin-bottom w3-card w3-row w3-display-container w3-border w3-round";
        root.setAttribute("index",i);
        if(docs[i].pinned) root.setAttribute("pinned","");
        if(docs[i].uploaded) root.setAttribute("uploaded","");
        
        // PIN
        let pin = document.createElement("div");
        pin.className = "w3-display-topright";
        let pinButton = document.createElement("button");
        pinButton.setAttribute("onclick","onclickPin(this)");
        if(docs[i].pinned) pinButton.className = "w3-button w3-round w3-ripple w3-grey";
        else pinButton.className = "w3-button w3-round w3-ripple";
        let pinIcon = document.createElement("i");
        pinIcon.className = "fa fa-thumb-tack";
        pinButton.appendChild(pinIcon);
        pin.appendChild(pinButton);
        root.appendChild(pin);

        // LIKES
        let rootVote = document.createElement("div");
        rootVote.className = "w3-display-bottomright";
        let buttonLike = document.createElement("button");
        buttonLike.setAttribute("onclick","onclickUp(this)");
        buttonLike.setAttribute("votes",docs[i].up);
        buttonLike.className = "w3-button w3-hover-green w3-round w3-text-green w3-ripple";
        buttonLike.innerHTML = "<i class='fa fa-arrow-up'></i> " + docs[i].up;
        let buttonDislike = document.createElement("button");
        buttonDislike.setAttribute("onclick","onclickDown(this)");
        buttonDislike.setAttribute("votes",docs[i].down);
        buttonDislike.className = "w3-button w3-hover-red w3-round w3-text-red w3-ripple";
        buttonDislike.innerHTML = "<i class='fa fa-arrow-down'></i> " + docs[i].down;
        rootVote.appendChild(buttonLike);
        rootVote.appendChild(buttonDislike);
        root.appendChild(rootVote);

        // IMAGE PART
        let leftRoot = document.createElement("div");
        leftRoot.className="w3-quarter";
        let img = document.createElement("img");
        img.className = "w3-round w3-small w3-margin-top img-max-width";
        img.src = "data/img/"+docs[i].preview;
        let buttonOpen = document.createElement("button");
        buttonOpen.setAttribute("onclick","onclickOpen(this)");
        buttonOpen.className="w3-button w3-margin-bottom w3-margin-top w3-bar w3-teal w3-hover-indigo w3-round w3-ripple";
        buttonOpen.innerHTML="OPEN";
        leftRoot.appendChild(img);
        leftRoot.appendChild(buttonOpen);
        root.appendChild(leftRoot);

        // LEFT PART
        let rightRoot = document.createElement("div");
        rightRoot.className = "w3-rest w3-margin-top";
            // INFO
        let infoRoot = document.createElement("div");
        infoRoot.className = "w3-bar w3-small w3-text-grey";
        let uploaded = document.createElement("div");
        uploaded.className = "w3-bar-item";
        uploaded.innerHTML = "Uploaded by: "+ docs[i].uploadedBy;
        let upload = document.createElement("div");
        upload.className = "w3-bar-item";
        upload.innerHTML = "Upload date: "+ docs[i].uploadDate;
        infoRoot.appendChild(uploaded);
        infoRoot.appendChild(upload);
            // NAME
        let name = document.createElement("b");
        name.className = "w3-margin-left w3-xlarge";
        name.innerHTML = docs[i].name;
        
            // TAGS
        let rootTags = document.createElement("p");
        let bar = document.createElement("div");
        bar.className = "w3-bar w3-margin-left";

        docs[i].tags.forEach(tag => bar.appendChild(getTagWithVoting(tag, docs[i].tags)));

        rootTags.appendChild(bar);

        // DESCRIPTION
        let desc = document.createElement("p");
        desc.className = "w3-margin-left w3-medium";
        desc.innerHTML = docs[i].description;

        rightRoot.appendChild(infoRoot);
        rightRoot.appendChild(name);
        rightRoot.appendChild(rootTags);
        rightRoot.appendChild(desc);
        root.appendChild(rightRoot);

        parent.appendChild(root);
    }
}
