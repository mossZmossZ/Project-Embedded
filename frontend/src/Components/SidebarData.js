import React from "react";
import HomeIcon from '@mui/icons-material/Home';
import AppRegistrationIcon from '@mui/icons-material/AppRegistration';
import ListIcon from '@mui/icons-material/List';
import HandshakeIcon from '@mui/icons-material/Handshake';
import PersonIcon from '@mui/icons-material/Person';

export const SidebarData = [
    {
        title:"Home",
        icon:<HomeIcon/>,
        link:"/"
    },

    {
        title:"Register",
        icon:<AppRegistrationIcon/>,
        link:"/register"
    },
    {
        title:"Borrowed",
        icon:<HandshakeIcon/>,
        link:"/Borrowlist"

    },
    {
        title:"Item List",
        icon:<ListIcon/>,
        link:"/itemlist"
    },

    {
        title:"Students",
        icon:<PersonIcon/>,
        link:"/Students"

    }
]
