namespace iepproj.Models
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Bid")]
    public partial class Bid
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public Bid()
        {
            Aukcijas = new HashSet<Aukcija>();
        }

        public int BidID { get; set; }

        public double? PonCena { get; set; }

        public DateTime? Vreme { get; set; }

        public int KorisnikID { get; set; }

        public int AukcijaID { get; set; }

        public virtual Aukcija Aukcija { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<Aukcija> Aukcijas { get; set; }

        public virtual Korisnik Korisnik { get; set; }
    }
}
