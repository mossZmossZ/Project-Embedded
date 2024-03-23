import React from "react";
import HomeIcon from '@mui/icons-material/Home';
import AppRegistrationIcon from '@mui/icons-material/AppRegistration';
import ListIcon from '@mui/icons-material/List';

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
        title:"Item List",
        icon:<ListIcon/>,
        link:"/itemlist"
    }
]
