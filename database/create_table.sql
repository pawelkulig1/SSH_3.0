DROP TABLE IF EXISTS ssh_keys;
DROP TABLE IF EXISTS ssh_log;
DROP TABLE IF EXISTS ssh_config;

CREATE TABLE ssh_keys (
    id                INT AUTO_INCREMENT PRIMARY KEY,
	host_name         VARCHAR(255),
	pub_key           TEXT,
	pub_key_converted TEXT,
	last_renewal      TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	renewable_by	  TIMESTAMP DEFAULT 0,
	valid_through     TIMESTAMP DEFAULT 0,
	INDEX(pub_key_converted)
) ENGINE=MyISAM;

DROP TABLE IF EXISTS ssh_log;

CREATE TABLE ssh_log (
	id        INT AUTO_INCREMENT PRIMARY KEY,
	user      VARCHAR(255),
	from_ip   VARCHAR(15),
	from_port INT,
	occured   timestamp DEFAULT CURRENT_TIMESTAMP
) ENGINE=MyISAM;

CREATE TABLE ssh_config (
	id 					INT AUTO_INCREMENT PRIMARY KEY,
	validity_period		INT DEFAULT 10368000, /*4 months*/
	renewability_period INT DEFAULT 7776000   /*3 months*/
) ENGINE=MyISAM;

insert into ssh_config (validity_period, renewability_period) values (10368000, 7776000);
insert into ssh_keys (host_name, pub_key, pub_key_converted) values ('root', 'ssh-rsa aaaab3nzac1yc2eaaaadaqabaaabgqdzyfskc4udbaimf30manv6jc8k/5zpqarliidlzgtfqmkwyyv4hjuljwrnyl3cik+jbpwgunrj1tp6fdo6gjnmmgd3+ck9ixrxxy55qkxom1r0frbqrlzc6xcqghzhlacmxcnopy3+wr2iem6w0z2zdnjrexo1is8rhkxes08g43ke9xnc9zvdmtzvdfwn64qw30okxs5vdyv65p16db+6n2v1c30lw3hzapstajusjbmfbh54mk1ar6ylzl/mh0lwytrlxbo43ystpn6vsfbryqu+236ysj2s57+6eeo8ilffzprf+1m3rfljoc7pglil7xyezoijooqlxt1ep2vehuxwbqusimhqbbeuf38xhf9knd7pk/sizw65gp6aienl4rl1jqqro0++quatcloshida9bzfktpioov84w60wxh4nfwa70houcxhapldixbhpmsapsfka22msds3c7fkjutfdnh4vpvyw9zg+7qkssrejrm7+w40i3fvtktk1vs= pablo@localhost.localdomain', 'f361fb24738b8305a88c177d2668dbfa8dcf0aff9ce941aacb2080cb6464c5aa6930cb2578849525270acd625ddc8a4f896cf586ba7ae3d53a7a15da3a8233663060f7f822bd897af1c72e794245ce335af47d106a44b642e970901a16612da70cc5c368a72dfec11d88126eb0d33d997678d17b13b522cf111e4c5e4b4f20e37904f	719dcf5955d32dcef75f5a7eb8a96df438a5d2e6f0f257ae69d7a75bfba9f6575737d255b71f3029493009b928db31f6e1e7830ad5aafac8bce5fe61f497062dacb5c1a38dd8b133cdeafb1f6d1610bbedb7eb2b23d92e7bfba784a3c88b145ccf445fb59b745f96339cecf18b8a5ed7604cce889a0e425c53d5ea7654486e5d60505128a61d06c17947f7f1785ff4a9c3ee92bf488670eb918fe8021e9e5e112f58eaaaba34fbea946ad0a53921c8740f41cc592d3e2a28bfce30eb4c171f89c5c1aef48685025e168f2c3231047a4cb1aa5214a036d8c49d4b70bb7e426eb5f0e787854fbf2c3dcc6fbbaa4b2c45e8d19bbf96e348b77d54e44cad55b');
insert into ssh_keys (host_name, pub_key, pub_key_converted) values ('root', 'ssh-rsa aaaab3nzac1yc2eaaaadaqabaaabgqdzyfskc4udbaimf30manv6jc8k/5zpqarliidlzgtfqmkwyyv4hjuljwrnyl3cik+jbpwgunrj1tp6fdo6gjnmmgd3+ck9ixrxxy55qkxom1r0frbqrlzc6xcqghzhlacmxcnopy3+wr2iem6w0z2zdnjrexo1is8rhkxes08g43ke9xnc9zvdmtzvdfwn64qw30okxs5vdyv65p16db+6n2v1c30lw3hzapstajusjbmfbh54mk1ar6ylzl/mh0lwytrlxbo43ystpn6vsfbryqu+236ysj2s57+6eeo8ilffzprf+1m3rfljoc7pglil7xyezoijooqlxt1ep2vehuxwbqusimhqbbeuf38xhf9knd7pk/sizw65gp6aienl4rl1jqqro0++quatcloshida9bzfktpioov84w60wxh4nfwa70houcxhapldixbhpmsapsfka22msds3c7fkjutfdnh4vpvyw9zg+7qkssrejrm7+w40i3fvtktk1vs= pablo2@localhost.localdomain', 'f361fb24738b8305a88c177d2668dbfa8dcf0aff9ce941aacb2080cb6464c5aa6930cb2578849525270acd625ddc8a4f896cf586ba7ae3d53a7a15da3a8233663060f7f822bd897af1c72e794245ce335af47d106a44b642e970901a16612da70cc5c368a72dfec11d88126eb0d33d997678d17b13b522cf111e4c5e4b4f20e37904f719dcf5955d32dcef75f5a7eb8a96df438a5d2e6f0f257ae69d7a75bfba9f6575737d255b71f3029493009b928db31f6e1e7830ad5aafac8bce5fe61f497062dacb5c1a38dd8b133cdeafb1f6d1610bbedb7eb2b23d92e7bfba784a3c88b145ccf445fb59b745f96339cecf18b8a5ed7604cce889a0e425c53d5ea7654486e5d60505128a61d06c17947f7f1785ff4a9c3ee92bf488670eb918fe8021e9e5e112f58eaaaba34fbea946ad0a53921c8740f41cc592d3e2a28bfce30eb4c171f89c5c1aef48685025e168f2c3231047a4cb1aa5214a036d8c49d4b70bb7e426eb5f0e787854fbf2c3dcc6fbcaa4b2c45e8d19bbf96e348b77d54e44cad55b');
insert into ssh_keys (host_name, pub_key, pub_key_converted) values ('root', 'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQC7PhzdQHRNJ9w0b3H7HMAJ7wb7Is3rnPQUkhS3ryPyHCx1PBsc0CClQeY1Zd4WqaPKTgVx0Z6SADqB3uZybciFmGbirp1H3aqEys7phgh7RdMjNPkQHmWJ8TbwoEFtj0ozFdKok9/khDmoIOp7gMwBQd+1+oFBi1yIUlZD4QaLdDvgmVD3nNwX+i+pw0lofX2JSSpnLVxpJZT69/gGolzusArGPO7Rc5eTDeMhFGoQ34bIefYIi84Sg+o84oNv5ycjQgjh+NDUTyz0MdTBhp+JABSNKXQjBfGXZIlW2ThrlMRODLLoIZZJrGR5nwVxObjV0fzV5q8SXCsTZSxzeNSzRWXvQJhJgqgE/zOQVTvg/2wB7wQdi7DPVyheydtyT1O/4N5yg2Z5LNOT0xerOCfjhojud3644DeBrc/mot6QqCcQJMyTD8YGYhv7QU5ANooYfZ//F4fTbCDOpzxM9W33bTyh0fuEkhOL8J68NYO1I/crgy8Bk7Oko7LFKe50ci8= root@0ed09a5c1407', 'C2A42CA628085BDE653749B45A6381D0D6E0036B7A1614CF1F74F532281AB0B8B5302DC4EBEB5DA534673F14249729C0BD5B290A7F29A440740C58B8E2576D409B084A055EE33EDD94774F81B52DB1A999F8DD4C7C9137934823A8DFBC182F380500EC643A22B5B448D61C97A9230B03F9F3535E8987733C03A3420BFE541DC2D920C8C532F06A0C84B5C65EE9904EA524DD073BEDC77166B2047FF217903045174C47F3FAE78A1FC6A7CD51118AE8A38EDE5B71410F8DE0D625E382A10AF84AF14CC34F1B1C2D9806E4B64224EAA37413EAC3D07C6BFCB3ACE6E73C2C077511C1467B5C4F1B6050F0EF2BB44CCADF531DEBBF43AEC27FA71E21C4C49B214813');
insert into ssh_keys (host_name, pub_key, pub_key_converted) values ('root', 'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCp8f/CCd2wdHtKKJ7LAOqUn+b3YTtXzBFWGB4Or6y94jYuZtjJHslkLZqmxc1U8TkBmDXQe6tfz7jjIeOL3n27yzaV/C1zZrCqCZSpFKC0+OztaIBb86j1ObpfkqUTmN8c+gAL8Yfu5kZQTaTR7DWgYN2EUVARtwIzZpkQKd/lebCw8vyF4rT+b7+RIEtjRfihS1ZRstzyqdMhkqQmu2wcILAE7yjtco6TSvLDswBS0RP+L5foL8zCE89iHoQ4q1Ea//zhEeT0v4ZwRJ/s7So4y1g2Y3QuzeJrTt50abddXeIXQ49uCFhKVU1H7k2Ts27xjE2ib9NZ5tsF5eSSD6TOiJUAQ4NNlvHWsfliZIN/XCPMd/kxW3idS0cNV9mzph6Ow6Teu7VADVgz1K/GANBag8v3UzJy9Vh4QJb3lXCvAbUZEPgV1qVP0hI97UW9hm8eRi/PH4f899N1NC5Zo1gpPjJKW2W0P72K5upNQuNq8VfZMiitWnxI9iDW4Hn5AlU= pablo@localhost.localdomain', 'A9F1FFC209DDB0747B4A289ECB00EA949FE6F7613B57CC1156181E0EAFACBDE2362E66D8C91EC9642D9AA6C5CD54F139019835D07BAB5FCFB8E321E38BDE7DBBCB3695FC2D7366B0AA0994A914A0B4F8ECED68805BF3A8F539BA5F92A51398DF1CFA000BF187EEE646504DA4D1EC35A060DD84515011B7023366991029DFE579B0B0F2FC85E2B4FE6FBF91204B6345F8A14B5651B2DCF2A9D32192A426BB6C1C20B004EF28ED728E934AF2C3B30052D113FE2F97E82FCCC213CF621E8438AB511AFFFCE111E4F4BF8670449FECED2A38CB583663742ECDE26B4EDE7469B75D5DE217438F6E08584A554D47EE4D93B36EF18C4DA26FD359E6DB05E5E4920FA4CE88950043834D96F1D6B1F96264837F5C23CC77F9315B789D4B470D57D9B3A61E8EC3A4DEBBB5400D5833D4AFC600D05A83CBF7533272F558784096F79570AF01B51910F815D6A54FD2123DED45BD866F1E462FCF1F87FCF7D375342E59A358293E324A5B65B43FBD8AE6EA4D42E36AF157D93228AD5A7C48F620D6E079F90255');
insert into ssh_keys (host_name, pub_key, pub_key_converted) values ('root', 
'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCp8f/CCd2wdHtKKJ7LAOqUn+b3YTtXzBFWGB4Or6y94jYuZtjJHslkLZqmxc1U8TkBmDXQe6tfz7jjIeOL3n27yzaV/C1zZrCqCZSpFKC0+OztaIBb86j1ObpfkqUTmN8c+gAL8Yfu5kZQTaTR7DWgYN2EUVARtwIzZpkQKd/lebCw8vyF4rT+b7+RIEtjRfihS1ZRstzyqdMhkqQmu2wcILAE7yjtco6TSvLDswBS0RP+L5foL8zCE89iHoQ4q1Ea//zhEeT0v4ZwRJ/s7So4y1g2Y3QuzeJrTt50abddXeIXQ49uCFhKVU1H7k2Ts27xjE2ib9NZ5tsF5eSSD6TOiJUAQ4NNlvHWsfliZIN/XCPMd/kxW3idS0cNV9mzph6Ow6Teu7VADVgz1K/GANBag8v3UzJy9Vh4QJb3lXCvAbUZEPgV1qVP0hI97UW9hm8eRi/PH4f899N1NC5Zo1gpPjJKW2W0P72K5upNQuNq8VfZMiitWnxI9iDW4Hn5AlU= pablo@localhost.localdomain', 'B768AC3FA8B77C60838AF27173B248D77C3D617E380A0FAD91FEFE479ECF9065CBC4FC41EC42E396376181B747F3233FD8071EF14B0438A20E56F42C3AE2BB2C7781905302A65C9F53EFEB97EE5322AB51F098C24EEB3E3F5D3F4EFE79FF00F354C6171595CED54AF491BE010DDCF85673C3F116F1A50F4F8E8AFBE475854F6D34EE2EF64C51C7C06CEC9F71199E5C11FB7758FE23C84E6A2E6630228D5E58299E179671B1D53DAD3ABFA4D1CB6F57EA54640B4404059CF621972CC6E806D884F79429B9C85ED25AA9CD665502728E351279CD4336BD8CAC9FDF853B8E59867B50BB6ABB6C65C2BD47DB428977957735D84DB551B2FD33335C1B2382C20E4410D25ED195F6CFD3533CC83C7510B2BA01A6E156C71A330FE5FEA02BA54D290C8C18DC3D6B4DD1AE3E3D8FEDF1FCF70674980A3F017D7D55244F3852E9410222DB77D3E7B667E650BE76A2D0131ED6CFDF2F219B8D72082197E3CC23FD19DA64B1F0B6D3DB37262816227AD6DA74081C0814DA2F2EF31022C4C1FB5DAFBB99D061');
insert into ssh_keys (host_name, pub_key, pub_key_converted) values ('root', 
'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCpCt62Nh1FJ48JeCoVNlfMRq2LdQdSx9TWcwaGbBZ9Q8wi9qK73/fqg58F0BjnSg5/HaBqPurBbbEbKf23BHswIeAfrQlgCzcHlIKxFruTgLNb4M6hktv3Dxy/BY7HcbiifR+9DqTKxn+QqBSNqncekboMdWKNcwZNaEloSde0a3lbFiX6D4/hqQfh722dQqiMgJrahZmR4azDuqVhexUJFxHXwhm2LBL2oGUkloV+ZsKGtS5swk5EU/HCyO+gJjSyNDxvPIIGI+qKrW06G/62zURc/QaTq9/Ou3vQwQ0W5hw843eOTkmLr5CX7d6ZtvyoeOUflZ2nKWas0voXDeKa9vozbAqA+8aa77+uCseHXj1n5itNVfSUi+6fvSVw6GmqmekdqHC2ijypSJwX6iFravRRdgZqy98OARx6Am8dZ1z0iNzBGGCm9Am2ulnmYna3sc5BpIc1qXzWp6N+oKr7K+wTaaFQC+H9ol7loAILs2N+a2zbAJSJwfC1u1JIxwk= root@97fd92ae6e13', 'B768AC3FA8B77C60838AF27173B248D77C3D617E380A0FAD91FEFE479ECF9065CBC4FC41EC42E396376181B747F3233FD8071EF14B0438A20E56F42C3AE2BB2C7781905302A65C9F53EFEB97EE5322AB51F098C24EEB3E3F5D3F4EFE79FF00F354C6171595CED54AF491BE010DDCF85673C3F116F1A50F4F8E8AFBE475854F6D34EE2EF64C51C7C06CEC9F71199E5C11FB7758FE23C84E6A2E6630228D5E58299E179671B1D53DAD3ABFA4D1CB6F57EA54640B4404059CF621972CC6E806D884F79429B9C85ED25AA9CD665502728E351279CD4336BD8CAC9FDF853B8E59867B50BB6ABB6C65C2BD47DB428977957735D84DB551B2FD33335C1B2382C20E4410D25ED195F6CFD3533CC83C7510B2BA01A6E156C71A330FE5FEA02BA54D290C8C18DC3D6B4DD1AE3E3D8FEDF1FCF70674980A3F017D7D55244F3852E9410222DB77D3E7B667E650BE76A2D0131ED6CFDF2F219B8D72082197E3CC23FD19DA64B1F0B6D3DB37262816227AD6DA74081C0814DA2F2EF31022C4C1FB5DAFBB99D061');