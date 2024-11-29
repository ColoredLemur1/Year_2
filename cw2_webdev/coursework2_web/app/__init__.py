from flask import Flask, request, session
from flask_admin import Admin
from flask_babel import Babel
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_restful import Api

def get_locale():
    if request.args.get('lang'):
        session['lang'] = request.args.get('lang')
    return session.get('lang', 'en')


app = Flask(__name__)
app.config.from_object('config')
db = SQLAlchemy(app)
migrate = Migrate(app, db)
api = Api(app)
babel = Babel(app, locale_selector=get_locale)
admin = Admin(app,template_mode='bootstrap4')

from app import views, models, resources

# Register your API resources here
api.add_resource(resources.YourResource, '/<string:task_id>')
