using iepproj.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using PagedList;

namespace iepproj.Controllers
{
    public class NarudzbinaController : Controller
    {
        Model1 db = new Model1();
        // GET: Narudzbina
        public ActionResult Index(int? paket)
        {
            if (paket > 0 && paket < 4 && Session["ID"] != null)
            {
                Narudzbina n = new Narudzbina();
                n.KorisnikID = Int32.Parse(Session["ID"].ToString());
                n.DatumPravljenja = DateTime.UtcNow;
                n.Status="Cekanje na obradu";
                switch (paket)
                {
                    case 1: n.BrojTokena = 3;
                        n.CenaPaketa = 3;
                        break;
                    case 2: n.BrojTokena = 5;
                        n.CenaPaketa = 5;
                        break;
                    case 3: n.BrojTokena = 10;
                        n.CenaPaketa = 10;
                        break;
                }
                db.Narudzbinas.Add(n);
                db.SaveChanges();
                return RedirectToAction("MojeNarudzbine","Narudzbina");
            }
            return View();
        }
        public ActionResult MojeNarudzbine(int? page)
        {
            if(Session["ID"]!=null){
                int id=Int32.Parse(Session["ID"].ToString());;
                var n = db.Narudzbinas.Where(a=>a.KorisnikID==id);
                int pageNumber = (page ?? 1);
                n=n.OrderByDescending(s=>s.DatumPravljenja);
                return View(n.ToPagedList(pageNumber,10));
            }
            return RedirectToAction("Index", "Home");
        }
        public ActionResult Narudzbina(int id)
        {
            Narudzbina n = db.Narudzbinas.Find(id);
            int idk = Int32.Parse(Session["ID"].ToString()); ;
            if (n.KorisnikID != idk)
            {
                return RedirectToAction("Index","Home");
            }
            return View(n);
        }
    }
}