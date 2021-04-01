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
 - install MDSplus on your host machine.
   - directions at: 
   - for mac: 
     ```
       wget https://www.mdsplus.org/dist/macosx/alpha/MDSplus-alpha-7-126-2-osx.pkg
       double click it to install
     ```
     - add an default_tree_path to /usr/local/mdsplus/local/envsyms
       - 
        ```
          cat - >> /usr/local/mdsplus/local/envsyms
          default_tree_path   $HOME/trees/~t
          ^C
        ```
     - create a directory for the tree bagel
       - `mkdir -p $HOME/trees/bagel
     - create a bagel tree
       -
       ```
       . /usr/local/mdsplus/setup.sh
       export PY_DEVICE_PATH=/usr/local/mdsplus/pydevices
       mdstcl
       TCL> edit bagel
       TCL> add node marte /model=MARTE2_SUPERVISOR
       TCL> add node input /model=MARTE2_STREAM_IN
       TCL> add node output /model=MARTE2_STREAM
       TCL> add node controller /model=MARTE2_SIMULINK_GENERIC
       TCL> write
       TCL> close
       TCL> exit
       ``` 
- or make a VM to host mdsplus
  - create a UB machine
  - change its hostname to mdsserver
    - echo mdsserver > /etc/hostname
  - install openssh-server `sudo apt-get install openssh-server`
  - ansible-playbook -v mdsserver.yml
    - provide your password
    - hit return to use same pw for sudo
  - ssh to the machine and make your tree
    - ssh -Y mdsserver
  - create the bagel tree
   -
   ```
   mdstcl
   TCL> edit bagel
   TCL> add node marte /model=MARTE2_SUPERVISOR
   TCL> add node input /model=MARTE2_STREAM_IN
   TCL> add node output /model=MARTE2_STREAM
   TCL> add node controller /model=MARTE2_SIMULINK_GENERIC
   TCL> write
   TCL> close
   TCL> exit
   ```
 - now use jTraverser2 to set up the devices.

