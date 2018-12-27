# -*- coding: utf-8 -*-

from flask import Flask, Blueprint
from flask import render_template, redirect, session
from flask import jsonify, request

app = Flask(__name__, 
    template_folder='templates', 
    static_folder='templates/static',
    static_url_path='/static')

@app.route('/', methods=['GET'])
def app_index():
    if 'user' in session:
        return redirect('/user')
    return redirect('/home')

home = Blueprint( 'home', __name__,
    template_folder='vtemplates', 
    static_folder='vtemplates/vstatic',
    static_url_path='/vstatic'
    )

@home.route('/home', defaults={'path': ''}, methods=['GET'])
@home.route('/home/<path:path>', methods=['GET'])
def home_index(path):
    return render_template('home.html')

user = Blueprint( 'user', __name__,
    template_folder='vtemplates', 
    static_folder='vtemplates/vstatic',
    static_url_path='/vstatic'
    )

@user.route('/user', defaults={'path': ''}, methods=['GET'])
@user.route('/user/<path:path>', methods=['GET'])
def user_index(path):
    return render_template('user.html')

app.register_blueprint(home)
app.register_blueprint(user)

api = Blueprint( 'api', __name__ )

@api.route('/home/signin', methods=['POST'])
def home_signin():
    username = request.form.get('username')
    password = request.form.get('password')
    resp = { 'status': 'success' }
    if username == 'test' and password == '1234':
        session['user'] = username
    else:
        resp['status'] = 'fail'

    return jsonify(resp)

app.register_blueprint(api, url_prefix='/api')
app.secret_key = 'test'
if __name__ == '__main__':
    app.run(debug=True)