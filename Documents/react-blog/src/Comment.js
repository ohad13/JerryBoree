import React, { useState, useEffect } from 'react';

const Comment = ({ comments, blog, onAddComment }) => {
    const [name, setName] = useState('');
    const [commentText, setCommentText] = useState('');
    const [localComments, setLocalComments] = useState(comments);
    const handleSubmit = (e) => {
        e.preventDefault();
        if (commentText.trim() === '' || name.trim() === '') {
            return; // Do not add empty comments
        }
        const newComment = { id: Date.now(), name: name, text: commentText }; // You can generate a unique ID like this
        setLocalComments([...localComments, newComment]);
        onAddComment(newComment);
        setCommentText('');
    };

    useEffect(() => {
        setLocalComments(comments);
    }, [comments]);

    return (
        <div className="comment-section">
            <h4>Comments:</h4>            
            <ul>
                {Array.isArray(localComments) && localComments.length > 0 ? (
                    localComments.map((comment, index) => (
                        <div key={comment.id} className="comment">
                            <p>{index + 1}) {blog.author.toLowerCase()==comment.name.toLowerCase()? "Author" : comment.name }: {comment.text}</p>
                        </div> 
                        
                    ))
                ) : (
                    <p>No comments yet. Be the first to response.</p>
                )}
            </ul>
            <form onSubmit={handleSubmit}>
                <br />
                <label>Name: </label>
                <input placeholder='Enter your name'
                    onChange={(e) => setName(e.target.value)}
                    value={name} ></input>
                <textarea
                    rows="3"
                    cols="70"
                    placeholder="Add your comment..."
                    value={commentText}
                    onChange={(e) => setCommentText(e.target.value)}
                ></textarea>
                <br />
                <button type="submit">Submit Comment</button>
            </form>
        </div>
    );
};

export default Comment;
