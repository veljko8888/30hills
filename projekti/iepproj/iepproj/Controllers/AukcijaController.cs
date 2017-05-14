using iepproj.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Data.Entity;
using System.Globalization;

namespace iepproj.Controllers
{
    public class AukcijaController : Controller
    {
        private Model1 db= new Model1();
        // GET: Aukcija
        public ActionResult Index()
        {
            return View();
        }
        public ActionResult NapraviAukciju()
        {
            if (Session["Admin"]==null)
            {
                return RedirectToAction("Index", "Home");
            }
            return View();
        }
    
        [HttpPost]
        public ActionResult NapraviAukciju(String Proizvod,String Trajanje, String PocetnaCena,HttpPostedFileBase upload)
        {
            if (Proizvod != null && Trajanje != "" && PocetnaCena!="" && upload != null && upload.ContentLength > 0 )
            {
                byte[] image = new byte[upload.ContentLength];
                upload.InputStream.Read(image, 0, image.Length);
                double x = Double.Parse(PocetnaCena);
                x = (double)Math.Round((Decimal)x, 3, MidpointRounding.AwayFromZero);
                var newAuction = new Aukcija()
                {
                    Proizvod = Proizvod,
                    Trajanje = Int32.Parse(Trajanje),
                    PocetnaCena = x,
                    VremeKreiranja = DateTime.UtcNow,
                    Status = "READY",
                    TrenutnaCena = x,
                    Slika = image
                };
                db.Aukcijas.Add(newAuction);
                db.SaveChanges();
                ViewBag.Message = "Uspesno";
                return View();
             
            }
            else
            {
                ViewBag.Message = "Doslo je do greske";
                return View();
            }
            return View();
        }

        //[HttpPost]
        public ActionResult IzmeniAukciju(int id){
            if (Session["Admin"] == null)
            {
                return RedirectToAction("Index", "Home");
            }
            Aukcija a = db.Aukcijas.Find(id);
            if (a.Status != "READY")
            {
                return RedirectToAction("Index", "Home");
            }
            return View(a);
        }

        [HttpPost]
        public ActionResult IzmeniAukciju(String Proizvod, String Trajanje, String PocetnaCena, String Status,int id,HttpPostedFileBase upload)
        {
            if (Session["Admin"] == null)
            {
                return RedirectToAction("Index", "Home");
            }
            var a = db.Aukcijas.Find(id);
            if (Proizvod != null && Trajanje != "" && PocetnaCena!="" && Status != null && id != 0 && (Status=="OPEN"||Status=="READY"))
            {
                
                byte[] img=null;
                if (upload != null && upload.ContentLength > 0)
                {
                    byte[] image = new byte[upload.ContentLength];
                    upload.InputStream.Read(image, 0, image.Length);
                    img = image;
                }
                if (img!=null) a.Slika = img;
                a.Proizvod = Proizvod;
                a.Trajanje = Int32.Parse(Trajanje);
                Double x = Single.Parse(PocetnaCena);
                if (Status == "OPEN")
                {
                    DateTime startTime = DateTime.UtcNow;
                    DateTime endTime = startTime.AddSeconds((int)a.Trajanje);
                    a.VremeOtvaranja = startTime;
                    a.VremeZatvaranja = endTime;
                    
                }
                x =(Double) Math.Round((Decimal)x , 3, MidpointRounding.AwayFromZero);
                a.PocetnaCena = x;
                a.Status = Status;
                a.TrenutnaCena = x;
                db.Entry(a).State = EntityState.Modified;
                ViewBag.Message = "Uspesno";
                db.SaveChanges();
                
                return View(a);
            }
            else
            {
                ViewBag.Message = "Doslo je do greske";

                return View(a);
            }
        }

        public ActionResult ObrisiAukciju(int id)
        {
            if (Session["Admin"] == null)
            {
                return RedirectToAction("Index", "Home");
            }
            Aukcija a = db.Aukcijas.Find(id);
            a.Status = "DRAFT";
            db.Entry(a).State = EntityState.Modified;
            db.SaveChanges();
            return RedirectToAction("Index","Home");
        }

        public String Licitiraj(int id,out String ime,out double Cena,int idk)
        {
            ime = "Nemate dovoljno tokena";
            Cena = 0;
                Aukcija a = db.Aukcijas.Find(id);
                Korisnik k = db.Korisniks.Find(idk);
                if (k.BrojTokena > 0)
                {
                    if (a.Status == "OPEN")
                    {
                        Bid b = new Bid();
                        b.AukcijaID = a.AukcijaID;
                        b.KorisnikID = idk;
                        b.PonCena = a.TrenutnaCena + 0.5;
                        a.TrenutnaCena = b.PonCena;
                        b.Vreme = DateTime.UtcNow;
                        TimeSpan? t=a.VremeZatvaranja-b.Vreme;
                        String s = "OK";
                        if (t.Value.TotalSeconds<10)
                        {
                            a.Trajanje = a.Trajanje + 10;
                            DateTime startTime = a.VremeOtvaranja.Value;
                            DateTime endTime = startTime.AddSeconds((int)a.Trajanje);
                            a.VremeZatvaranja = endTime;
                            s = "DODAJ";
                        }
                        
                        ime = db.Korisniks.Find(b.KorisnikID).Ime + " " + db.Korisniks.Find(b.KorisnikID).Prezime;
                        Cena = (double)a.TrenutnaCena;
                        db.Bids.Add(b);
                        db.SaveChanges();
                        return s;
                    }
                    return "CLOSED";
                }
                return "TOKEN";
        }
        public ActionResult AukcijaFull(int id)
        {
            Aukcija a = db.Aukcijas.Find(id);
            //var res = (from b in db.Bids where b.AukcijaID==a.AukcijaID orderby b.Vreme descending select new { Bid = b }).Take(10);
            if (Session["ID"] != null)
            {
                var kor = db.Korisniks.Find(Session["ID"]);
                ViewBag.idk = Session["ID"].ToString();
                ViewBag.brtokena = kor.BrojTokena;
            }
            ViewBag.users = new String[10];
            int i = 0;
            var bids = db.Bids.Include(b => b.Korisnik);
            bids = bids.Where(s => s.AukcijaID==a.AukcijaID);
            bids=bids.OrderByDescending(s => s.Vreme);
            bids=bids.Take(10);
            foreach (var bx in bids)
            {
                int kid = bx.KorisnikID;
                ViewBag.users[i++] = db.Korisniks.Find(kid).Ime +" "+ db.Korisniks.Find(kid).Prezime+ " " + bx.PonCena ;
            }
            if (a.VremeZatvaranja != null) { ViewBag.vreme = (a.VremeZatvaranja.Value - DateTime.UtcNow).TotalSeconds; }
            return View(a);
        }
        public String Otkucalo(int id)
        {
            String s = "";
            Aukcija a = db.Aukcijas.Find(id);
            if (a.Status == "OPEN" && a.VremeZatvaranja.Value<DateTime.UtcNow)
            {
                if (a.BidID != null)
                {
                    a.Status = "SOLD";
                }
                else
                {
                    a.Status = "EXPIRED";
                }
                db.Entry(a).State = EntityState.Modified;
                db.SaveChanges();
                s = a.Status;
                
            }
            return s;
        }
    }
}