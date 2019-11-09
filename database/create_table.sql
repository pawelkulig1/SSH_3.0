
CREATE TABLE IF NOT EXISTS ssh_keys (
    id INT AUTO_INCREMENT PRIMARY KEY,
	host_name VARCHAR(255),
	pub_key TEXT,
	pub_key_converted TEXT,
	last_renewal TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	valid_through TIMESTAMP DEFAULT 0
) ENGINE=INNODB;

INSERT INTO ssh_keys (host_name, pub_key, pub_key_converted) VALUES ('root', 'ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQDzYfskc4uDBaiMF30maNv6jc8K/5zpQarLIIDLZGTFqmkwyyV4hJUlJwrNYl3cik+JbPWGunrj1Tp6Fdo6gjNmMGD3+CK9iXrxxy55QkXOM1r0fRBqRLZC6XCQGhZhLacMxcNopy3+wR2IEm6w0z2ZdnjRexO1Is8RHkxeS08g43kE9xnc9ZVdMtzvdfWn64qW30OKXS5vDyV65p16db+6n2V1c30lW3HzApSTAJuSjbMfbh54MK1ar6yLzl/mH0lwYtrLXBo43YsTPN6vsfbRYQu+236ysj2S57+6eEo8iLFFzPRF+1m3RfljOc7PGLil7XYEzOiJoOQlxT1ep2VEhuXWBQUSimHQbBeUf38Xhf9KnD7pK/SIZw65GP6AIenl4RL1jqqro0++qUatClOSHIdA9BzFktPioov84w60wXH4nFwa70hoUCXhaPLDIxBHpMsapSFKA22MSdS3C7fkJutfDnh4VPvyw9zG+7qkssRejRm7+W40i3fVTkTK1Vs= pablo@localhost.localdomain', 'F361FB24738B8305A88C177D2668DBFA8DCF0AFF9CE941AACB2080CB6464C5AA6930CB2578849525270ACD625DDC8A4F896CF586BA7AE3D53A7A15DA3A8233663060F7F822BD897AF1C72E794245CE335AF47D106A44B642E970901A16612DA70CC5C368A72DFEC11D88126EB0D33D997678D17B13B522CF111E4C5E4B4F20E37904F719DCF5955D32DCEF75F5A7EB8A96DF438A5D2E6F0F257AE69D7A75BFBA9F6575737D255B71F3029493009B928DB31F6E1E7830AD5AAFAC8BCE5FE61F497062DACB5C1A38DD8B133CDEAFB1F6D1610BBEDB7EB2B23D92E7BFBA784A3C88B145CCF445FB59B745F96339CECF18B8A5ED7604CCE889A0E425C53D5EA7654486E5D60505128A61D06C17947F7F1785FF4A9C3EE92BF488670EB918FE8021E9E5E112F58EAAABA34FBEA946AD0A53921C8740F41CC592D3E2A28BFCE30EB4C171F89C5C1AEF48685025E168F2C3231047A4CB1AA5214A036D8C49D4B70BB7E426EB5F0E787854FBF2C3DCC6FBBAA4B2C45E8D19BBF96E348B77D54E44CAD55B');
