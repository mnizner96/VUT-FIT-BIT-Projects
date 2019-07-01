<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of Database
 *
 * @author nizner
 */
class Database
{

	const HOST_LOCAL = '127.0.0.1';
	const DBNAME_LOCAL = 'dev_kapo-odevy';
	const HOST_MOONLAKE = 'mysql1.moonlake.local';
	const DBNAME_MOONLAKE = 'dev_nellys';

	private $dsn_local = 'mysql:dbname=' . self::DBNAME_LOCAL . ';host=' . self::HOST_LOCAL . ';charset=utf8';
	private $user_local = 'root';
	private $pass_local = '';
	private $dsn_moonlake = 'mysql:dbname=' . self::DBNAME_MOONLAKE . ';host=' . self::HOST_MOONLAKE . ';charset=utf8';
	private $user_moonlake = 'martin.nizner';
	private $pass_moonlake = 'XZ25zEduWaxBUtZu';
	private $db_local, $db_moonlake;

	public function __construct()
	{
		try {
			$this->db_local = new PDO($this->dsn_local, $this->user_local, $this->pass_local);
			$this->db_local->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			$this->db_moonlake = new PDO($this->dsn_moonlake, $this->user_moonlake, $this->pass_moonlake);
			$this->db_moonlake->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		} catch (PDOException $ex) {
			die('Connection failed: ' . $ex->getMessage());
		}
	}

	public function testSpeed($query)
	{
		$starttime_local = microtime(true);
		//try {
		//$this->db_local->query($query);
		//} catch (PDOException $ex) {
		//die('Query executin failed: ' . $ex->getMessage());
		//}
		$endtime_local = microtime(true);
		$duration_local = $endtime_local - $starttime_local;

		$starttime_moonlake = microtime(true);
		try {
			$this->db_moonlake->query($query);
		} catch (PDOException $ex) {
			die('Query executin failed: ' . $ex->getMessage());
		}
		$endtime_moonlake = microtime(true);
		$duration_moonlake = $endtime_moonlake - $starttime_moonlake;

		echo ("Total query time of " . $query . " on local database was " . $duration_local . " s\n");
		echo ("Total query time of " . $query . " on moonlake database was " . $duration_moonlake . " s\n");
		$difference = $duration_moonlake - $duration_local;
		echo ("Difference between them (moonlake database time - local database time) is " . $difference . " s");
	}

	//put your code here
}
