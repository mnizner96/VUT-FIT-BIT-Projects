<?php

require_once('Database.php');
$db = new Database();
$options = getopt('q:');
$db->testSpeed($options['q']);


