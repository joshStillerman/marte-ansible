## Ansible script for MARTe2 / MDSplus
- To use:
  - make a ub machine and install ssh on it
  - on your host computer pip install ansible
  - then clone this repo
  - my VM ended up with a host name ubuntu on my network. if yours did not edit the file hosts in the repo
  - `ansible-playbook -v marte.yml`
    — provide your password for the UB machine
    — hit return for the sudo password (it will use the previously provided)
    — provide the host name of your mdsip server when prompted
