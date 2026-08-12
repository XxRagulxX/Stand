function simplify_key(key)
{
	key = key.toLowerCase();
	if(key.substr(0,5)=="stand")
	{
		key=key.substr(5);
	}
	if(key.substr(0,1)=="-")
	{
		key=key.substr(1);
	}
	if(key.substr(0,9)=="activate-")
	{
		key=key.substr(9);
	}
	return key;
}

function time()
{
	return Math.floor(Date.now() / 1000);
}

function update_activation_key(key)
{
	if(localStorage.getItem("activation_key")==localStorage.getItem("web_key"))
	{
		localStorage.removeItem("web_key");
	}
	localStorage.setItem("activation_key", key);
}

function store_account_data(data)
{
	if("account_id" in data)
	{
		localStorage.setItem("account_id", data.account_id);
	}
	if("activation_key" in data)
	{
		update_activation_key(data.activation_key);
		localStorage.setItem("last_account_data_update", time()); // Only update timer on a full update
	}
	if("privilege" in data)
	{
		localStorage.setItem("account_privilege", data.privilege);
	}
	if("suspended_for" in data)
	{
		localStorage.setItem("account_suspended_for", data.suspended_for);
	}
	if("coins" in data)
	{
		localStorage.setItem("account_coins", data.coins);
	}
	if("created_quiz_success" in data)
	{
		localStorage.setItem("account_created_quiz_success", data.created_quiz_success);
	}
	if("interested" in data)
	{
		localStorage.setItem("account_interested", data.interested);
	}
	if("pinkeyed" in data)
	{
		localStorage.setItem("account_pinkeyed", data.pinkeyed);
	}
}

function forget_account_data()
{
	localStorage.removeItem("activation_key");
	localStorage.removeItem("account_privilege");
	localStorage.removeItem("account_suspended_for");
	localStorage.removeItem("account_coins");
	localStorage.removeItem("account_created_quiz_success");
	localStorage.removeItem("account_interested");
	localStorage.removeItem("account_pinkeyed");
	localStorage.removeItem("last_account_data_update");
}

function format_coins(coins)
{
	return (coins / 100).toFixed(2).split(".").join(",");
}

function copy_to_clipboard(text)
{
	var input = document.createElement("textarea");
	input.innerHTML = text;
	document.body.appendChild(input);
	input.select();
	document.execCommand("copy");
	document.body.removeChild(input);
}

function ensure_account_info_is_up_to_date(logged_in_cb, logged_out_cb)
{
	if(typeof localStorage.getItem("account_id")!="string")
	{
		logged_out_cb();
		return;
	}
	if ((time() - localStorage.getItem("last_account_data_update")) > (60 * 60))
	{
		$.post("/api/basic_account_info", {
			account_id: localStorage.getItem("account_id")
		}).done(function(data)
		{
			if (data != "bad")
			{
				store_account_data(data);
				logged_in_cb();
			}
			else
			{
				logged_out_cb();
			}
		}).fail(function()
		{
			logged_out_cb();
		});
	}
	else
	{
		logged_in_cb();
	}
}
