#ifndef __SSH_AUTO_KEYGEN_H__
#define __SSH_AUTO_KEYGEN_H__

void do_gen_all_hostkeys();
struct sshbuf *generate_public_private_keys();

#endif
