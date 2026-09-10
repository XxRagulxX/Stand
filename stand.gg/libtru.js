const truIpToInt = (ip) => {
	const [a, b, c, d] = ip.split(".").map(Number);
	return ((a << 24) | (b << 16) | (c << 8) | d) >>> 0;
};
console.assert(truIpToInt("127.0.0.1") === 0x7F000001);

const truIntToIp = (ip) => {
	return `${(ip >>> 24) & 0xff}.${(ip >>> 16) & 0xff}.${(ip >>> 8) & 0xff}.${ip & 0xff}`;
};
console.assert(truIntToIp(0x7F000001) === "127.0.0.1");

const truEncodeInvite = (id, port, lanIps) => {
	const buffer = Buffer.alloc(7 + lanIps.length * 4);
	buffer.writeUInt32BE(id >>> 0, 0);
	buffer.writeUInt16BE(port, 4);
	buffer.writeUInt8(lanIps.length, 6);
	lanIps.forEach((ip, i) => {
		buffer.writeUInt32BE(truIpToInt(ip), 7 + i * 4);
	});
	return buffer;
};

const truDecodeInvite = (buffer) => {
	const id = buffer.readUInt32BE(0);
	const port = buffer.readUInt16BE(4);
	const num_lan_ips = buffer.readUInt8(6);
	let i = 7;
	const lan_ips = [];
	for (let j = 0; j != num_lan_ips; ++j) {
		const lan_ip = buffer.readUInt32BE(i);
		lan_ips.push(truIntToIp(lan_ip));
		i += 4;
	}
	if (buffer.length != i) {
		throw new Error("unexpected invite length");
	}
	return [id, port, lan_ips];
};

if (typeof Buffer != "undefined") {
	console.assert(truEncodeInvite(0xcafebabe, 0x1234, ["127.0.0.1"]).equals(Buffer.from([ 0xca, 0xfe, 0xba, 0xbe, 0x12, 0x34, 0x01, 0x7f, 0x00, 0x00, 0x01 ])));
	const [id, port, lan_ips] = truDecodeInvite(Buffer.from([ 0xca, 0xfe, 0xba, 0xbe, 0x12, 0x34, 0x01, 0x7f, 0x00, 0x00, 0x01 ]));
	console.assert(id === 0xcafebabe);
	console.assert(port === 0x1234);
	console.assert(lan_ips.length == 1);
	console.assert(lan_ips[0] === "127.0.0.1");
}


const hookWsReject = (ws, reject) => {
	ws.onerror = (_event) => {
		reject("WebSocket connection error");
	};
	ws.onclose = () => {
		reject("WebSocket connection closed");
	};
};

const openWs = (ip, port) => {
	const uri = (typeof location != "undefined" && location.protocol == "https:") // Is secure context?
		? `wss://${ip.replaceAll(".", "-").replaceAll(":", "_")}.faketls.com:${port}`
		: `ws://${ip}:${port}`;
	return new Promise((resolve, reject) => {
		const ws = new WebSocket(uri);
		ws.onopen = () => {
			resolve(ws);
		};
		hookWsReject(ws, reject);
	});
};

const tryLanConnection = (id, port, lan_ips) => {
	return Promise.any(
		lan_ips.map(ip => {
			return new Promise((resolve, reject) => {
				openWs(ip, port).then(ws => {
					hookWsReject(ws, reject);
					ws.onmessage = (event) => {
						if (event.data == id) {
							resolve(ws);
						}
					};
					ws.send(id);
				}).catch(reject);
			});
		})
	);
};

/**
 * @param {string} relay - relay server IP address
 * @param {string} id_or_invite
 * @param {function} [onwaiting] - called before resolve in the case where we're waiting for the host instead of the other way around
 * @returns {Promise<WebSocket>}
 */
const truConnect = (relay, id_or_invite, onwaiting) => {
	return new Promise((resolve, reject) => {
		let id = id_or_invite;
		new Promise((invite_resolve, invite_reject) => {
			const [id_int, port, lan_ips] = truDecodeInvite(Buffer.from(id_or_invite, "base64url"));
			id = id_int.toString(16);
			tryLanConnection(id, port, lan_ips).then(conn => {
				invite_resolve(conn);
			}).catch(invite_reject);
		}).then(resolve).catch(() => {
			openWs(relay, 7987).then(ctrl => {
				ctrl.onmessage = (event) => {
					//console.log("ctrl->client: " + event.data);
					if (event.data == "wait") {
						if (id == id_or_invite) {
							if (onwaiting) {
								onwaiting(id);
							}
						} else {
							ctrl.close();
							reject("The invite is no longer valid.");
						}
					} else if (event.data.startsWith("try ")) {
						if (id == id_or_invite) {
							const [port, lan_ips] = event.data.substr(4).split(" ");
							tryLanConnection(id, port, lan_ips.split(",")).then(conn => {
								ctrl.close();
								resolve(conn);
							}).catch(() => {
								ctrl.send("relay");
							});
						} else {
							ctrl.send("relay");
						}
					} else if (event.data == "relay") {
						resolve(ctrl);
					}
				};
				hookWsReject(ctrl, reject);
				ctrl.send(`c ${id}`);
			}).catch(reject);
		});
	});
};

if (typeof window != "undefined") {
	window.truIpToInt = truIpToInt;
	window.truIntToIp = truIntToIp;
	window.truEncodeInvite = truEncodeInvite;
	window.truDecodeInvite = truDecodeInvite;
	window.truConnect = truConnect;
} else {
	module.exports = {
		truIpToInt,
		truIntToIp,
		truEncodeInvite,
		truDecodeInvite,
		truConnect,
	};
}
