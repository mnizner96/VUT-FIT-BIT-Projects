namespace Gallery.DAL.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class AddLocationWidth : DbMigration
    {
        public override void Up()
        {
            AddColumn("dbo.Locations", "Width", c => c.Int(nullable: false));
            AddColumn("dbo.Locations", "Height", c => c.Int(nullable: false));
            DropColumn("dbo.Locations", "Position");
        }
        
        public override void Down()
        {
            AddColumn("dbo.Locations", "Position", c => c.String());
            DropColumn("dbo.Locations", "Height");
            DropColumn("dbo.Locations", "Width");
        }
    }
}
