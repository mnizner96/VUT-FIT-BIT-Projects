namespace Gallery.DAL.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class InitialCreate : DbMigration
    {
        public override void Up()
        {
            CreateTable(
                "dbo.Albums",
                c => new
                    {
                        Id = c.Guid(nullable: false),
                        Name = c.String(nullable: false),
                    })
                .PrimaryKey(t => t.Id);
            
            CreateTable(
                "dbo.Photographies",
                c => new
                    {
                        PhotographyId = c.Guid(nullable: false),
                        Image = c.Binary(nullable: false),
                        Name = c.String(nullable: false),
                        Time = c.DateTime(nullable: false),
                        Format = c.String(),
                        Height = c.Int(nullable: false),
                        Weight = c.Int(nullable: false),
                        Additional = c.String(),
                        Album_Id = c.Guid(),
                    })
                .PrimaryKey(t => t.PhotographyId)
                .ForeignKey("dbo.Albums", t => t.Album_Id)
                .Index(t => t.Album_Id);
            
            CreateTable(
                "dbo.Locations",
                c => new
                    {
                        Id = c.Guid(nullable: false),
                        Position = c.String(),
                        Photography_PhotographyId = c.Guid(),
                        Item_Id = c.Guid(),
                        Person_Id = c.Guid(),
                    })
                .PrimaryKey(t => t.Id)
                .ForeignKey("dbo.Photographies", t => t.Photography_PhotographyId)
                .ForeignKey("dbo.Items", t => t.Item_Id)
                .ForeignKey("dbo.People", t => t.Person_Id)
                .Index(t => t.Photography_PhotographyId)
                .Index(t => t.Item_Id)
                .Index(t => t.Person_Id);
            
            CreateTable(
                "dbo.Items",
                c => new
                    {
                        Id = c.Guid(nullable: false),
                        Name = c.String(),
                    })
                .PrimaryKey(t => t.Id);
            
            CreateTable(
                "dbo.People",
                c => new
                    {
                        Id = c.Guid(nullable: false),
                        FirstName = c.String(),
                        LastName = c.String(),
                    })
                .PrimaryKey(t => t.Id);
            
        }
        
        public override void Down()
        {
            DropForeignKey("dbo.Locations", "Person_Id", "dbo.People");
            DropForeignKey("dbo.Locations", "Item_Id", "dbo.Items");
            DropForeignKey("dbo.Locations", "Photography_PhotographyId", "dbo.Photographies");
            DropForeignKey("dbo.Photographies", "Album_Id", "dbo.Albums");
            DropIndex("dbo.Locations", new[] { "Person_Id" });
            DropIndex("dbo.Locations", new[] { "Item_Id" });
            DropIndex("dbo.Locations", new[] { "Photography_PhotographyId" });
            DropIndex("dbo.Photographies", new[] { "Album_Id" });
            DropTable("dbo.People");
            DropTable("dbo.Items");
            DropTable("dbo.Locations");
            DropTable("dbo.Photographies");
            DropTable("dbo.Albums");
        }
    }
}
