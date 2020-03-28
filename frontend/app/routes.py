from app import app
from db import *
from flask import render_template, request

@app.route('/')
@app.route('/index')
def index():
    base = DBConnector()
    keys = base.get_keys()
    return render_template('keys.html', table=keys)
    # return str(keys)

@app.route('/api', methods=['PUT', 'GET'])
def api():
    base = DBConnector()
    delete = request.args.get("delete")
    prolong = request.args.get("prolong")
    renew = request.args.get("force_renewal")
    disable = request.args.get("disable")
    pub_key = request.args.get("add_key")

    ret = False
    if delete is not None:
        ret = base.delete_key(delete)
    elif prolong is not None:
        ret = base.prolong(prolong)
    elif renew is not None:
        ret = base.force_renewal(renew)
    elif disable is not None:
        ret = base.disable_renewal(disable)
    elif pub_key is not None:
        ret = base.add_key(pub_key)
    return str(ret)
