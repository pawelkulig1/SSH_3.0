from app import app
import flask
from app.forms import LoginForm
import sys


import sys
from common.db import *

# login_manager = flask_login.LoginManager()
# login_manager.init_app(app)

# @login_manager.user_loader
def load_user(user_id):
    return 0



@app.route('/', methods=['PUT', 'GET', 'POST'])
@app.route('/index', methods=['PUT', 'GET', 'POST'])
@app.route('/keys', methods=['PUT', 'GET', 'POST'])
def index():
    q = ""
    p = 0
    query_m = flask.request.args.get("q")
    page = flask.request.args.get("p")
    if query_m is not None:
        q = query_m
    
    if page is not None:
        try:
            p = int(page)
        except:
            p = 0

    base = DBConnector()
    keys = base.search_keys(q, p)
    return flask.render_template('keys.html', table=keys)
    # return str(keys)

@app.route('/logs', methods=['PUT', 'GET', 'POST'])
def logs():
    q = ""
    p = 0
    query_m = flask.request.args.get("q")
    page = flask.request.args.get("p")
    if query_m is not None:
        q = query_m
    
    if page is not None:
        try:
            p = int(page)
        except:
            p = 0
    
    base = DBConnector()
    logs = base.search_logs(q, p)
    if isinstance(logs, str):
        return logs
    return flask.render_template('logs.html', table=logs)

@app.route('/login', methods=['GET', 'POST'])
def login():
    # Here we use a class of some kind to represent and validate our
    # client-side form data. For example, WTForms is a library that will
    # handle this for us, and we use a custom LoginForm to validate.
    form = LoginForm()
    if form.validate_on_submit():
        # Login and validate the user.
        # user should be an instance of your `User` class
        flask.login_user(user)

        flask.flash('Logged in successfully.')

        next = flask.request.args.get('next')
        # is_safe_url should check if the url is safe for redirects.
        # See http://flask.pocoo.org/snippets/62/ for an example.
        if not flask.is_safe_url(next):
            return flask.abort(400)

        return flask.redirect(next or flask.url_for('index'))
    return flask.render_template('login.html', form=form)

@app.route('/api', methods=['PUT', 'GET', 'POST'])
def api():
    base = DBConnector()
    delete = flask.request.args.get("delete")
    prolong = flask.request.args.get("prolong")
    renew = flask.request.args.get("force_renewal")
    disable = flask.request.args.get("disable")
    pub_key = flask.request.args.get("add_key")

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
        first_poz = pub_key.find("ssh-rsa ")
        last_space = pub_key.rfind("= ")
        pub_key = pub_key[:first_poz + 8] + \
                  pub_key[first_poz + 8: last_space].replace(" ", "+") + \
                  pub_key[last_space:]

        # print("public_converted:", pub_key, file=sys.stderr)
        ret = base.add_key(pub_key)
    return str(ret)
