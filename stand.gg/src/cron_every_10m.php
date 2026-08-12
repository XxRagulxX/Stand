<?php
if (empty($argv))
{
	http_response_code(418);
	exit;
}

require "dbinclude.php";

$db->query("DELETE FROM `menus` WHERE `last_heartbeat`<?", "i", time() - (31 * 60));

/*foreach (json_decode(file_get_contents("https://battleye.dudx.info/bans"), true) as $entry)
{
	$rid = $entry["rid"];
	if (!$db->query("SELECT `id` FROM `scaccounts` WHERE `id`=?", "i", $rid))
	{
		$db->query("INSERT IGNORE INTO `rid_queue` (`rid`) VALUES (?)", "i", $rid);
	}
}*/
