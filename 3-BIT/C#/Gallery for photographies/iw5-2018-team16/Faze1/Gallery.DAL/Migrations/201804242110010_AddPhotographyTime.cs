namespace Gallery.DAL.Migrations
{
    using System;
    using System.Data.Entity.Migrations;
    
    public partial class AddPhotographyTime : DbMigration
    {
        public override void Up()
        {
            AlterColumn("dbo.Photographies", "Time", c => c.String());
        }
        
        public override void Down()
        {
            AlterColumn("dbo.Photographies", "Time", c => c.DateTime(nullable: false));
        }
    }
}
