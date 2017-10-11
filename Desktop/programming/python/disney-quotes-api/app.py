from flask import Flask
from flask_restful import Resource, Api
import quote
import sys


app = Flask(__name__)
api = Api(app)

reload(sys)
sys.setdefaultencoding('utf-8')

class APP(Resource):
    def get(self):
        return {'under construction'}

    def post(self):
        q = quote.getQuote()
        #q= q.encode('ascii', 'ignore').decode('ascii')
        return q

class API(Resource):
    def get(self):
        q = quote.getQuote()
        #q = q.encode('ascii', 'ignore').decode('ascii')
        return q

api.add_resource(APP, '/')
api.add_resource(API, '/api')

if __name__ == '__main__':
    app.run(debug=True)