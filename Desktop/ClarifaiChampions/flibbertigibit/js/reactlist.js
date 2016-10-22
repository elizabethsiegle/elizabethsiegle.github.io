var ListItem = React.createClass({
    render: function() { //author, text
        return <li
            className="infinite-list-item">
            <a href={url} target="_blank">{this.props.tags}</a>
        </li>;
    }
   
});

var InfiniteList = React.createClass({
    getInitialState: function() {
        TweetStore.listen((msg) => {
            this.state.elements.push(msg.tags);
            if(this.state.elements.length > 100) {
                this.state.elements.shift();
            }
            this.setState({
                elements:this.state.elements
            });
        });
        return {
            elements: []
        }
    },

    render: function() {
        var elements = this.state.elements.map((tags,i)=>{
            return <ListItem key={i} tags={tags}/>
        });
        return <ul>{elements}</ul>;
    }

});



function makeReactList() {
    ReactDOM.render(<InfiniteList/>, document.getElementById('react-list'));
}