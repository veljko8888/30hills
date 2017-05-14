namespace iepproj.Models
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Narudzbina")]
    public partial class Narudzbina
    {
        public int NarudzbinaID { get; set; }

        public double? CenaPaketa { get; set; }

        public int? BrojTokena { get; set; }

        [StringLength(20)]
        public string Status { get; set; }

        public DateTime? DatumPravljenja { get; set; }

        public int KorisnikID { get; set; }

        public virtual Korisnik Korisnik { get; set; }
    }
}
