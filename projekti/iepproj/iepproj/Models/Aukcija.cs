namespace iepproj.Models
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Aukcija")]
    public partial class Aukcija
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public Aukcija()
        {
            Bids = new HashSet<Bid>();
            Korisniks = new HashSet<Korisnik>();
        }

        public int AukcijaID { get; set; }

        [Required]
        [StringLength(50)]
        public string Proizvod { get; set; }

        [Required]
        public int? Trajanje { get; set; }
        [Required]
        public double? PocetnaCena { get; set; }

        public DateTime? VremeKreiranja { get; set; }

        public DateTime? VremeOtvaranja { get; set; }

        public DateTime? VremeZatvaranja { get; set; }

        [StringLength(10)]
        public string Status { get; set; }

        public double? TrenutnaCena { get; set; }

        
        public byte[] Slika { get; set; }

        public int? BidID { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<Bid> Bids { get; set; }

        public virtual Bid Bid { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<Korisnik> Korisniks { get; set; }
    }
}
