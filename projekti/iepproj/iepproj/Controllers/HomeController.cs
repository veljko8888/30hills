using iepproj.Models;
using System;
using System.Data.Entity;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Web;
using System.Web.Mvc;
using PagedList;
using System.Collections.Generic;

namespace iepproj.Controllers
{
    public class HomeController : Controller
    {
        public static string GetSha256FromString(string strData)
        {
            var message = Encoding.ASCII.GetBytes(strData);
            SHA256Managed hashString = new SHA256Managed();
            string hex = "";

            var hashValue = hashString.ComputeHash(message);
            foreach (byte x in hashValue)
            {
                hex += String.Format("{0:x2}", x);
            }
            return hex;
        }
        private Model1 db = new Model1();
        public ActionResult Index(String searchString,String Min,String Max,String Status,int? page,int? moje)
        {
            ViewBag.CurrentFilter = searchString;
            ViewBag.min = Min;
            ViewBag.Max = Max;
            ViewBag.Status = Status;
            if (Session["ID"] != null)
            {
                var kor = db.Korisniks.Find(Session["ID"]);
                ViewBag.idk = Session["ID"].ToString();
                ViewBag.brtokena = kor.BrojTokena;
            }
           var aukcijas = db.Aukcijas.Include(a => a.Bid);
            /*var aukcijas = from user in db.Korisniks
                                      from auc in user.Aukcijas
                                      where user.KorisnikID == idk
                                      select auc;*/
           if (moje==1 && Session["ID"]!=null)
           {
               int idk = Int32.Parse(Session["ID"].ToString());
               var bids = db.Bids.Where(b => b.KorisnikID == idk);
               List<int> bidids=bids.Select(x=>x.BidID).ToList();
               aukcijas=aukcijas.Where(b=>bidids.Contains(b.BidID.Value));
               aukcijas = aukcijas.Where(s => s.Status.Equals("SOLD"));
           }
           else if (moje == 1)
           {
               moje = 0;
           }
           
           aukcijas = aukcijas.Where(s => !s.Status.Equals("DRAFT"));
           int pageSize = 8;
           if (!String.IsNullOrEmpty(searchString))
           {
               aukcijas = aukcijas.Where(s => s.Proizvod.Contains(searchString));
           }
           if (!String.IsNullOrEmpty(Min))
           {
               double x=Double.Parse(Min);
               aukcijas = aukcijas.Where(s => s.TrenutnaCena>=x);
           }
           if (!String.IsNullOrEmpty(Max))
           {
               double x = Double.Parse(Max);
               aukcijas = aukcijas.Where(s => s.TrenutnaCena <= x);
           }
           if (!String.IsNullOrEmpty(Status))
           {
               aukcijas = aukcijas.Where(s => s.Status ==Status);
           }
            
            int pagenumber=(page ?? 1);
            /*if (!String.IsNullOrEmpty(searchString) || !String.IsNullOrEmpty(Min) || !String.IsNullOrEmpty(Max) || !String.IsNullOrEmpty(Status))
            {
                pagenumber = 1;
                
            }*/
            if (String.IsNullOrEmpty(searchString) && String.IsNullOrEmpty(Min) && String.IsNullOrEmpty(Max) && String.IsNullOrEmpty(Status) && moje!=1)
            {
                aukcijas = aukcijas.Where(s => s.Status.Equals("OPEN"));
            }
            aukcijas = aukcijas.OrderByDescending(s => s.VremeOtvaranja);
            ViewBag.bid = new String[10000];
            ViewBag.vreme = new double[10000];
            foreach (var a in aukcijas)
            {
                if (a.BidID!=null)
                {
                    Bid b = db.Bids.Find(a.BidID);
                    Korisnik k = db.Korisniks.Find(b.KorisnikID);
                    ViewBag.bid[a.AukcijaID] = k.Ime + " " + k.Prezime;
                }
                else
                {
                    ViewBag.bid[a.AukcijaID] = "Nema licitacija";
                }
                if (a.VremeZatvaranja != null) { ViewBag.vreme[a.AukcijaID] = (a.VremeZatvaranja.Value - DateTime.UtcNow).TotalSeconds; }
            }
            return View(aukcijas.ToPagedList(pagenumber,pageSize));
        }
        public ActionResult Login()
        {
            return View();
        }
        public ActionResult Edit()
        {
            Korisnik k = db.Korisniks.Find(Session["ID"]);
            return View(k);
        }
        public ActionResult Register()
        {
            return View();
        }
        public ActionResult Logout()
        {
            Session.Abandon();
            Session.Clear();
            return RedirectToAction("Index");
        }
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Register([Bind(Include = "Ime,Prezime,Email,Lozinka")] Korisnik korisnik,String Potvrda)
        {
            
            if (ModelState.IsValid)
            {
                if (korisnik.Ime != null && korisnik.Prezime != null && korisnik.Lozinka != null && korisnik.Email != null && Potvrda!=null) 
                {
                    if (Potvrda != korisnik.Lozinka)
                    {
                        ViewBag.Message = "Password se ne poklapa";
                        return View();
                    }
                    var res = from k in db.Korisniks where k.Email == korisnik.Email select new { Korisnik = k };
                    if (res.Any())
                    {
                        ViewBag.Message = "Email vec postoji";
                        return View();
                    }
                    korisnik.Admin = false;
                    korisnik.BrojTokena = 0;
                    korisnik.Lozinka = GetSha256FromString(korisnik.Lozinka);
                    db.Korisniks.Add(korisnik);
                    db.SaveChanges();
                    return RedirectToAction("Login");
                }
                ViewBag.Message = "Popunite sva polja";
                return View();
            }
            ViewBag.Message = "Doslo je do greske";
            return View();
        }
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Login([Bind(Include="Email,Lozinka")]Korisnik k1)
        {
            if (k1.Lozinka != null && k1.Email != null)
            {
                String enpw = GetSha256FromString(k1.Lozinka);
                var res = from k in db.Korisniks where k.Email == k1.Email && k.Lozinka == enpw select new { Korisnik = k };
                if (res.Any())
                {
                    Session["Email"] = res.First().Korisnik.Email;
                    Session["ID"] = res.First().Korisnik.KorisnikID;
                    Session["Ime"] = res.First().Korisnik.Ime;
                    Session["Prezime"] = res.First().Korisnik.Prezime;
                    if (res.First().Korisnik.Admin.Value) Session["admin"] = 1;
                    return RedirectToAction("Index");
                }
                ViewBag.Message = "Pogresan email ili lozinka";
                return View();
            }
            ViewBag.Message = "Popunite sva polja";
            return View();
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Edit([Bind(Include = "Ime,Prezime,Email")] Korisnik korisnik,String Lozinka, String NovaLozinka, String PotvrdaNove)
        {

            if (ModelState.IsValid)
            {
                if (korisnik.Ime != null && korisnik.Prezime != null && korisnik.Email != null && Lozinka!="")
                {
                    int id = (int)Session["ID"];
                    var kor = db.Korisniks.Find(id);
                    if(kor.Lozinka!=GetSha256FromString(Lozinka)){
                        ViewBag.Message="Netacna lozinka";
                        return View();
                    }
                    var res = from k in db.Korisniks where k.Email == korisnik.Email && k.KorisnikID!=id select new { Korisnik = k };
                    if (res.Any())
                    {
                        ViewBag.Message = "Email vec postoji";
                        return View();
                    }
                    if (NovaLozinka != "")
                    {
                        if (NovaLozinka == PotvrdaNove)
                        {
                            kor.Lozinka = GetSha256FromString(NovaLozinka);
                        }
                        else
                        {
                            ViewBag.Message = "Password se ne poklapa";
                            return View();
                        }
                    }
                    kor.Ime = korisnik.Ime;
                    kor.Prezime = korisnik.Prezime;
                    kor.Email = korisnik.Email;
                    db.Entry(kor).State = EntityState.Modified;
                    db.SaveChanges();
                    return View();
                }
                ViewBag.Message = "Popunite sva polja";
                return View();
            }
            ViewBag.Message = "Doslo je do greske";
            return View();
        }
        
    }
}