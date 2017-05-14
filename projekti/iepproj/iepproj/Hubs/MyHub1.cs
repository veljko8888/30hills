using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Microsoft.AspNet.SignalR;
using iepproj.Controllers;
namespace iepproj.Hubs
{
    public class MyHub1 : Hub
    {
        public void Hello(String name)
        {

            Clients.All.addNewMessageToPage(name);
        }
        public void Licit(int id,int idk)
        {
            String ime;
            Double Cena;
            AukcijaController ac = new AukcijaController();
            String s=ac.Licitiraj(id, out ime, out Cena, idk);
            if (s=="OK")
            {
                Clients.All.Licitirano(id, ime, Cena,s);
                Clients.All.Tokeni(idk);
            }
            else if(s=="TOKEN")
            {
                Clients.Caller.nemaTokena();
            }
            else if (s == "DODAJ")
            {
                Clients.All.Licitirano(id, ime, Cena, s);
                Clients.All.Tokeni(idk);
            }
            else if (s == "CLOSED")
            {

            }
        }
        public void otkuc(int id) {
            AukcijaController ac = new AukcijaController();
            String s = ac.Otkucalo(id);
            Clients.All.otkucalo(id,s);
        }
    }
}