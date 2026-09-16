/* =========================================================
   SECURELINK
   ONE-PERSON SECURE COMMUNICATION SIMULATOR

   NORMAL PAGE
        ↓
   USER TYPES MESSAGE
        ↓
   SEND
        ↓
   FULL SCREEN ANIMATION
        ↓
   REAL AES-256-GCM CIPHERTEXT
        ↓
   AUTHENTICATION
        ↓
   DECRYPTION
        ↓
   SAME MESSAGE
        ↓
   SLIDE BACK TO NORMAL PAGE

   NO FAKE REPLY
   ========================================================= */


/* =========================================================
   ELEMENTS
   ========================================================= */

const transmissionScreen =
    document.getElementById(
        "transmissionScreen"
    );


const usernameInput =
    document.getElementById(
        "username"
    );


const friendNameInput =
    document.getElementById(
        "friendName"
    );


const connectButton =
    document.getElementById(
        "connectButton"
    );


const messageInput =
    document.getElementById(
        "messageInput"
    );


const sendButton =
    document.getElementById(
        "sendButton"
    );


const messages =
    document.getElementById(
        "messages"
    );


/* =========================================================
   STATUS ELEMENTS
   ========================================================= */

const connectionStatus =
    document.getElementById(
        "connectionStatus"
    );


const networkStatus =
    document.getElementById(
        "networkStatus"
    );


const chatTitle =
    document.getElementById(
        "chatTitle"
    );


const chatStatus =
    document.getElementById(
        "chatStatus"
    );


const securityText =
    document.getElementById(
        "securityText"
    );


const displayUsername =
    document.getElementById(
        "displayUsername"
    );


const friendDisplay =
    document.getElementById(
        "friendDisplay"
    );


const encryptionValue =
    document.getElementById(
        "encryptionValue"
    );


const integrityValue =
    document.getElementById(
        "integrityValue"
    );


const snrValue =
    document.getElementById(
        "snrValue"
    );


const berValue =
    document.getElementById(
        "berValue"
    );


/* =========================================================
   SIDE TRANSMISSION
   ========================================================= */

const sideTransmissionState =
    document.getElementById(
        "sideTransmissionState"
    );


const sideOriginal =
    document.getElementById(
        "sideOriginal"
    );


const sideCipher =
    document.getElementById(
        "sideCipher"
    );


const sideAuth =
    document.getElementById(
        "sideAuth"
    );


const sideFinal =
    document.getElementById(
        "sideFinal"
    );


/* =========================================================
   FULL SCREEN ANIMATION ELEMENTS
   ========================================================= */

const heroOriginalData =
    document.getElementById(
        "heroOriginalData"
    );


const heroCipherData =
    document.getElementById(
        "heroCipherData"
    );


const heroAuthData =
    document.getElementById(
        "heroAuthData"
    );


const heroDecryptData =
    document.getElementById(
        "heroDecryptData"
    );


const heroFinalData =
    document.getElementById(
        "heroFinalData"
    );


const introStatus =
    document.getElementById(
        "introStatus"
    );


const introProgress =
    document.getElementById(
        "introProgress"
    );


/* =========================================================
   STATE
   ========================================================= */

let encryptionKey = null;

let connected = false;

let animationRunning = false;


/* =========================================================
   SHA-256 → AES-256 KEY
   ========================================================= */

async function deriveEncryptionKey(
    password
)
{
    const encoder =
        new TextEncoder();


    const passwordData =
        encoder.encode(
            password
        );


    const hash =
        await crypto.subtle.digest(
            "SHA-256",
            passwordData
        );


    return crypto.subtle.importKey(
        "raw",
        hash,
        {
            name:
                "AES-GCM"
        },
        false,
        [
            "encrypt",
            "decrypt"
        ]
    );
}


/* =========================================================
   ARRAY BUFFER → BASE64
   ========================================================= */

function arrayBufferToBase64(
    buffer
)
{
    const bytes =
        new Uint8Array(
            buffer
        );


    let binary = "";


    for (
        let i = 0;
        i < bytes.length;
        i++
    )
    {
        binary +=
            String.fromCharCode(
                bytes[i]
            );
    }


    return btoa(
        binary
    );
}


/* =========================================================
   BASE64 → ARRAY BUFFER
   ========================================================= */

function base64ToArrayBuffer(
    base64
)
{
    const binary =
        atob(
            base64
        );


    const bytes =
        new Uint8Array(
            binary.length
        );


    for (
        let i = 0;
        i < binary.length;
        i++
    )
    {
        bytes[i] =
            binary.charCodeAt(i);
    }


    return bytes.buffer;
}


/* =========================================================
   AES-256-GCM ENCRYPTION
   ========================================================= */

async function encryptMessage(
    text
)
{
    if (!encryptionKey)
    {
        throw new Error(
            "Encryption key unavailable."
        );
    }


    const encoder =
        new TextEncoder();


    const plaintext =
        encoder.encode(
            text
        );


    /*
     * Fresh random 96-bit IV.
     */

    const iv =
        crypto.getRandomValues(
            new Uint8Array(12)
        );


    /*
     * Actual AES-GCM encryption.
     */

    const ciphertext =
        await crypto.subtle.encrypt(
            {
                name:
                    "AES-GCM",

                iv:
                    iv
            },

            encryptionKey,

            plaintext
        );


    return {

        iv:
            arrayBufferToBase64(
                iv
            ),

        ciphertext:
            arrayBufferToBase64(
                ciphertext
            )

    };
}


/* =========================================================
   AES-256-GCM DECRYPTION
   ========================================================= */

async function decryptMessage(
    ciphertextBase64,
    ivBase64
)
{
    if (!encryptionKey)
    {
        throw new Error(
            "Encryption key unavailable."
        );
    }


    const ciphertext =
        base64ToArrayBuffer(
            ciphertextBase64
        );


    const iv =
        base64ToArrayBuffer(
            ivBase64
        );


    /*
     * If ciphertext was modified,
     * AES-GCM authentication fails here.
     */

    const plaintext =
        await crypto.subtle.decrypt(
            {
                name:
                    "AES-GCM",

                iv:
                    new Uint8Array(
                        iv
                    )
            },

            encryptionKey,

            new Uint8Array(
                ciphertext
            )
        );


    const decoder =
        new TextDecoder();


    return decoder.decode(
        plaintext
    );
}


/* =========================================================
   DELAY
   ========================================================= */

function delay(
    milliseconds
)
{
    return new Promise(
        function(resolve)
        {
            setTimeout(
                resolve,
                milliseconds
            );
        }
    );
}


/* =========================================================
   ACTIVATE HERO STEP
   ========================================================= */

function activateHeroStep(
    id
)
{
    const element =
        document.getElementById(
            id
        );


    if (!element)
    {
        return;
    }


    element.classList.add(
        "active"
    );
}


/* =========================================================
   COMPLETE HERO STEP
   ========================================================= */

function completeHeroStep(
    id
)
{
    const element =
        document.getElementById(
            id
        );


    if (!element)
    {
        return;
    }


    element.classList.remove(
        "active"
    );


    element.classList.add(
        "completed"
    );
}


/* =========================================================
   ACTIVATE ARROW
   ========================================================= */

function activateHeroArrow(
    index
)
{
    const arrows =
        document.querySelectorAll(
            ".hero-arrow"
        );


    if (
        index >= 0 &&
        index < arrows.length
    )
    {
        arrows[index].classList.add(
            "active"
        );
    }
}


/* =========================================================
   RESET FULL SCREEN ANIMATION
   ========================================================= */

function resetHeroAnimation()
{
    const steps =
        document.querySelectorAll(
            ".hero-step"
        );


    const arrows =
        document.querySelectorAll(
            ".hero-arrow"
        );


    steps.forEach(
        function(step)
        {
            step.classList.remove(
                "active",
                "completed"
            );
        }
    );


    arrows.forEach(
        function(arrow)
        {
            arrow.classList.remove(
                "active"
            );
        }
    );


    introProgress.style.width =
        "0%";


    introStatus.textContent =
        "STARTING SECURE TRANSMISSION";


    heroOriginalData.textContent =
        "Waiting...";


    heroCipherData.textContent =
        "Generating...";


    heroAuthData.textContent =
        "Verifying...";


    heroDecryptData.textContent =
        "Waiting...";


    heroFinalData.textContent =
        "Waiting...";
}


/* =========================================================
   SHOW FULL SCREEN ANIMATION
   ========================================================= */

function showTransmissionScreen()
{
    resetHeroAnimation();


    transmissionScreen.classList.remove(
        "exit"
    );


    transmissionScreen.classList.add(
        "show"
    );
}


/* =========================================================
   HIDE FULL SCREEN ANIMATION
   ========================================================= */

async function hideTransmissionScreen()
{
    transmissionScreen.classList.add(
        "exit"
    );


    await delay(1000);


    transmissionScreen.classList.remove(
        "show",
        "exit"
    );
}


/* =========================================================
   CONNECT / START SESSION
   ========================================================= */

connectButton.addEventListener(
    "click",
    async function()
    {
        const username =
            usernameInput.value.trim();


        const friend =
            friendNameInput.value.trim();


        if (
            username === ""
        )
        {
            alert(
                "Please enter your name."
            );

            usernameInput.focus();

            return;
        }


        if (
            friend === ""
        )
        {
            alert(
                "Please enter your friend's name."
            );

            friendNameInput.focus();

            return;
        }


        const password =
            prompt(
                "Enter a security password.\n\n" +
                "Use at least 8 characters."
            );


        if (
            password === null
        )
        {
            return;
        }


        if (
            password.length < 8
        )
        {
            alert(
                "Password must contain at least 8 characters."
            );

            return;
        }


        try
        {
            encryptionKey =
                await deriveEncryptionKey(
                    password
                );


            connected = true;


            displayUsername.textContent =
                username;


            friendDisplay.textContent =
                friend;


            chatTitle.textContent =
                friend;


            connectionStatus.textContent =
                "Demo Mode";


            networkStatus.textContent =
                "Standalone • Local";


            chatStatus.textContent =
                "Secure demonstration ready";


            securityText.textContent =
                "Secure channel active";


            encryptionValue.textContent =
                "AES-256-GCM";


            integrityValue.textContent =
                "READY";


            connectButton.textContent =
                "Session Active";


            messageInput.disabled =
                false;


            sendButton.disabled =
                false;


            updateConnectionIndicators(
                true
            );


            removeWelcomeCard();


            addSystemMessage(
                "Secure session established."
            );


            addSystemMessage(
                "Ready for encrypted transmission."
            );


            messageInput.focus();

        }
        catch(error)
        {
            console.error(
                error
            );


            alert(
                "Encryption initialization failed."
            );
        }
    }
);


/* =========================================================
   SEND BUTTON
   ========================================================= */

sendButton.addEventListener(
    "click",
    sendMessage
);


/* =========================================================
   ENTER KEY
   ========================================================= */

messageInput.addEventListener(
    "keydown",
    function(event)
    {
        if (
            event.key ===
            "Enter"
        )
        {
            event.preventDefault();

            sendMessage();
        }
    }
);


/* =========================================================
   SEND MESSAGE
   ========================================================= */

async function sendMessage()
{
    if (
        !connected ||
        !encryptionKey ||
        animationRunning
    )
    {
        return;
    }


    const text =
        messageInput.value.trim();


    if (
        text === ""
    )
    {
        return;
    }


    const username =
        usernameInput.value.trim() ||
        "You";


    animationRunning = true;


    /*
     * Disable input during animation.
     */

    sendButton.disabled =
        true;

    messageInput.disabled =
        true;


    try
    {
        /* =========================================
           ACTUAL ENCRYPTION
           ========================================= */

        const encrypted =
            await encryptMessage(
                text
            );


        /*
         * Show the user's message in chat.
         *
         * NO RECEIVED REPLY.
         */

        addMessage(
            text,
            "sent",
            username
        );


        messageInput.value =
            "";


        /* =========================================
           UPDATE SIDE PANEL
           ========================================= */

        sideOriginal.textContent =
            text;


        sideCipher.textContent =
            encrypted.ciphertext;


        sideAuth.textContent =
            "Waiting...";


        sideFinal.textContent =
            "Waiting...";


        sideTransmissionState.textContent =
            "READY";


        /* =========================================
           SHOW FULL SCREEN
           ========================================= */

        showTransmissionScreen();


        await delay(450);


        /* =========================================
           STEP 1
           ORIGINAL MESSAGE
           ========================================= */

        activateHeroStep(
            "heroOriginal"
        );


        heroOriginalData.textContent =
            text;


        introStatus.textContent =
            "ORIGINAL MESSAGE";


        introProgress.style.width =
            "10%";


        await delay(1000);


        activateHeroArrow(0);


        await delay(350);


        completeHeroStep(
            "heroOriginal"
        );


        /* =========================================
           STEP 2
           AES-256-GCM
           ========================================= */

        activateHeroStep(
            "heroEncryption"
        );


        introStatus.textContent =
            "AES-256-GCM ENCRYPTION";


        introProgress.style.width =
            "25%";


        await delay(1100);


        activateHeroArrow(1);


        await delay(350);


        completeHeroStep(
            "heroEncryption"
        );


        /* =========================================
           STEP 3
           REAL CIPHERTEXT
           ========================================= */

        activateHeroStep(
            "heroCipher"
        );


        heroCipherData.textContent =
            encrypted.ciphertext;


        introStatus.textContent =
            "REAL CIPHERTEXT GENERATED";


        introProgress.style.width =
            "40%";


        await delay(1400);


        activateHeroArrow(2);


        await delay(350);


        completeHeroStep(
            "heroCipher"
        );


        /* =========================================
           STEP 4
           TRANSMISSION
           ========================================= */

        activateHeroStep(
            "heroTransmission"
        );


        sideTransmissionState.textContent =
            "TRANSMITTING";


        introStatus.textContent =
            "SIMULATED TRANSMISSION";


        introProgress.style.width =
            "55%";


        await delay(1500);


        activateHeroArrow(3);


        await delay(350);


        completeHeroStep(
            "heroTransmission"
        );


        /* =========================================
           STEP 5
           AUTHENTICATION
           ========================================= */

        activateHeroStep(
            "heroAuthentication"
        );


        heroAuthData.textContent =
            "VERIFYING...";


        sideAuth.textContent =
            "VERIFYING...";


        introStatus.textContent =
            "VERIFYING AUTHENTICITY";


        introProgress.style.width =
            "70%";


        integrityValue.textContent =
            "VERIFYING";


        await delay(1100);


        /*
         * Actual decryption is attempted.
         *
         * AES-GCM authentication is verified
         * during this operation.
         */

        let decrypted;


        try
        {
            decrypted =
                await decryptMessage(
                    encrypted.ciphertext,
                    encrypted.iv
                );
        }
        catch(error)
        {
            throw new Error(
                "Authentication failed."
            );
        }


        heroAuthData.textContent =
            "✓ AUTHENTICATION PASSED";


        sideAuth.textContent =
            "✓ PASSED";


        integrityValue.textContent =
            "PASSED";


        securityText.textContent =
            "Authentication verified";


        introStatus.textContent =
            "AUTHENTICATION PASSED";


        await delay(1000);


        activateHeroArrow(4);


        await delay(350);


        completeHeroStep(
            "heroAuthentication"
        );


        /* =========================================
           STEP 6
           DECRYPTION
           ========================================= */

        activateHeroStep(
            "heroDecryption"
        );


        heroDecryptData.textContent =
            "Decrypting ciphertext...";


        introStatus.textContent =
            "DECRYPTING";


        introProgress.style.width =
            "85%";


        await delay(1100);


        heroDecryptData.textContent =
            "✓ Message recovered";


        securityText.textContent =
            "Message decrypted";


        await delay(700);


        activateHeroArrow(5);


        await delay(350);


        completeHeroStep(
            "heroDecryption"
        );


        /* =========================================
           FINAL STEP
           ========================================= */

        activateHeroStep(
            "heroFinal"
        );


        heroFinalData.textContent =
            decrypted;


        sideFinal.textContent =
            decrypted;


        sideTransmissionState.textContent =
            "COMPLETE";


        introStatus.textContent =
            "SECURE TRANSMISSION COMPLETE";


        introProgress.style.width =
            "100%";


        integrityValue.textContent =
            "VERIFIED";


        securityText.textContent =
            "Secure transmission completed";


        await delay(2200);


        /* =========================================
           SLIDE AWAY
           ========================================= */

        await hideTransmissionScreen();


        /*
         * Return to normal interface.
         */

        messageInput.disabled =
            false;


        sendButton.disabled =
            false;


        animationRunning =
            false;


        messageInput.focus();

    }
    catch(error)
    {
        console.error(
            "Secure transmission error:",
            error
        );


        heroAuthData.textContent =
            "✕ AUTHENTICATION FAILED";


        sideAuth.textContent =
            "✕ FAILED";


        sideTransmissionState.textContent =
            "FAILED";


        integrityValue.textContent =
            "FAILED";


        securityText.textContent =
            "Authentication failed";


        await delay(1500);


        await hideTransmissionScreen();


        messageInput.disabled =
            false;


        sendButton.disabled =
            false;


        animationRunning =
            false;
    }
}


/* =========================================================
   ADD SENT MESSAGE
   ========================================================= */

function addMessage(
    text,
    type,
    sender
)
{
    const message =
        document.createElement(
            "div"
        );


    message.className =
        "message " +
        type;


    const messageText =
        document.createElement(
            "div"
        );


    messageText.textContent =
        text;


    const messageInfo =
        document.createElement(
            "div"
        );


    messageInfo.className =
        "message-info";


    messageInfo.textContent =
        sender +
        " • " +
        new Date().toLocaleTimeString(
            [],
            {
                hour:
                    "2-digit",

                minute:
                    "2-digit"
            }
        );


    message.appendChild(
        messageText
    );


    message.appendChild(
        messageInfo
    );


    messages.appendChild(
        message
    );


    scrollMessages();
}


/* =========================================================
   SYSTEM MESSAGE
   ========================================================= */

function addSystemMessage(
    text
)
{
    const message =
        document.createElement(
            "div"
        );


    message.className =
        "system-message";


    message.textContent =
        text;


    messages.appendChild(
        message
    );


    scrollMessages();
}


/* =========================================================
   REMOVE WELCOME
   ========================================================= */

function removeWelcomeCard()
{
    const card =
        document.querySelector(
            ".welcome-card"
        );


    if (card)
    {
        card.remove();
    }
}


/* =========================================================
   SCROLL
   ========================================================= */

function scrollMessages()
{
    messages.scrollTop =
        messages.scrollHeight;
}


/* =========================================================
   CONNECTION INDICATORS
   ========================================================= */

function updateConnectionIndicators(
    connectedState
)
{
    const statusIndicator =
        document.querySelector(
            ".status-indicator"
        );


    const contactDot =
        document.querySelector(
            ".contact-dot"
        );


    const onlineDot =
        document.querySelector(
            ".online-dot"
        );


    if (connectedState)
    {
        if (statusIndicator)
        {
            statusIndicator.style.background =
                "#58e6a8";

            statusIndicator.style.boxShadow =
                "0 0 10px rgba(88,230,168,0.7)";
        }


        if (contactDot)
        {
            contactDot.style.background =
                "#58e6a8";
        }


        if (onlineDot)
        {
            onlineDot.style.background =
                "#58e6a8";

            onlineDot.style.boxShadow =
                "0 0 8px rgba(88,230,168,0.6)";
        }
    }
}


/* =========================================================
   NAVIGATION
   ========================================================= */

const navigationItems =
    document.querySelectorAll(
        ".nav-item"
    );


navigationItems.forEach(
    function(item)
    {
        item.addEventListener(
            "click",
            function()
            {
                navigationItems.forEach(
                    function(nav)
                    {
                        nav.classList.remove(
                            "active"
                        );
                    }
                );


                item.classList.add(
                    "active"
                );


                const section =
                    item.dataset.section;


                if (
                    section ===
                    "chat"
                )
                {
                    document
                        .querySelector(
                            ".chat-section"
                        )
                        .scrollIntoView({
                            behavior:
                                "smooth"
                        });
                }


                if (
                    section ===
                    "security"
                )
                {
                    document
                        .querySelector(
                            ".security-card"
                        )
                        .scrollIntoView({
                            behavior:
                                "smooth"
                        });
                }


                if (
                    section ===
                    "analysis"
                )
                {
                    document
                        .querySelector(
                            ".side-transmission"
                        )
                        .scrollIntoView({
                            behavior:
                                "smooth"
                        });
                }

            }
        );
    }
);


/* =========================================================
   INITIALIZE
   ========================================================= */

function initializeInterface()
{
    connected = false;

    animationRunning = false;

    encryptionKey = null;


    /*
     * IMPORTANT:
     *
     * The normal interface remains visible.
     * The transmission screen stays hidden.
     */

    transmissionScreen.classList.remove(
        "show",
        "exit"
    );


    messageInput.disabled =
        true;


    sendButton.disabled =
        true;


    connectionStatus.textContent =
        "Offline";


    networkStatus.textContent =
        "Not connected";


    chatStatus.textContent =
        "Waiting for secure connection";


    securityText.textContent =
        "Awaiting connection";


    encryptionValue.textContent =
        "AES-256-GCM";


    integrityValue.textContent =
        "Waiting";


    snrValue.textContent =
        "--";


    berValue.textContent =
        "--";


    sideTransmissionState.textContent =
        "READY";


    sideOriginal.textContent =
        "Waiting...";


    sideCipher.textContent =
        "Waiting...";


    sideAuth.textContent =
        "Waiting...";


    sideFinal.textContent =
        "Waiting...";


    updateConnectionIndicators(
        false
    );
}


/* =========================================================
   START
   ========================================================= */

initializeInterface();