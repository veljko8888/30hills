namespace iepproj.Models
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class Model1 : DbContext
    {
        public Model1()
            : base("name=Model11")
        {

        }

        public virtual DbSet<Aukcija> Aukcijas { get; set; }
        public virtual DbSet<Bid> Bids { get; set; }
        public virtual DbSet<Korisnik> Korisniks { get; set; }
        public virtual DbSet<Narudzbina> Narudzbinas { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            /*modelBuilder.Entity<Aukcija>()
                .Property(e => e.Slika)
                .IsFixedLength();*/

            modelBuilder.Entity<Aukcija>()
                .HasMany(e => e.Bids)
                .WithRequired(e => e.Aukcija)
                .HasForeignKey(e => e.AukcijaID)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Bid>()
                .HasMany(e => e.Aukcijas)
                .WithOptional(e => e.Bid)
                .HasForeignKey(e => e.BidID);

            modelBuilder.Entity<Korisnik>()
                .HasMany(e => e.Bids)
                .WithRequired(e => e.Korisnik)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Korisnik>()
                .HasMany(e => e.Narudzbinas)
                .WithRequired(e => e.Korisnik)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Korisnik>()
                .HasMany(e => e.Aukcijas)
                .WithMany(e => e.Korisniks)
                .Map(m => m.ToTable("Dobio").MapLeftKey("KorisnikID").MapRightKey("AukcijaID"));
        }
    }
}
