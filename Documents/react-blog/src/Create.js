import { useState } from "react";
import { useHistory } from "react-router-dom";

const Create = () => {
    const [title, setTitle] = useState('')
    const [body, setBody] = useState('')
    const [comments, setComments] = useState('')
    const [image, setImage] = useState('')
    const [like, setLike] = useState(0)
    const [author, setAuthor] = useState('mario')
    const [language, setLanguage] = useState('en');
    const nowTime = new Date().toLocaleDateString();
    const [time, setTime] = useState(nowTime);
    const [isPanding, setIsPending] = useState(false);
    const history = useHistory();
    const [newAuthor, setNewAuthor] = useState('');
    const [authorOptions, setOptions] = useState([
        { value: 'mario', label: 'Mario' },
        { value: 'yoshi', label: 'Yoshi' },
    ]);
    const [languageOptions, setLanguageOptions] = useState([
        { value: 'en', label: 'English' },
        { value: 'es', label: 'Spanish' },
        { value: 'fr', label: 'French' },
        { value: 'de', label: 'German' },
        { value: 'it', label: 'Italian' },
        { value: 'pt', label: 'Portuguese' },
        { value: 'zh', label: 'Chinese' },
        { value: 'ja', label: 'Japanese' },
        { value: 'ko', label: 'Korean' },
        { value: 'ru', label: 'Russian' },
        { value: 'ar', label: 'Arabic' },
    ]);
    const [isAddingNewAuthor, setIsAddingNewAuthor] = useState(false);

    const predefinedImages = [
        'images/blog1.jpg',
        'images/blog2.jpg',
        'images/blog3.jpg',
        'images/blog4.jpg',
    ];
    const [chosenImage, setChosenImage] = useState(predefinedImages[0]);
    const handleImageClick = (imagePath) => {
        setChosenImage(imagePath);
        setImage(imagePath);
    };

    const handleAddAuthor = () => {
        if (newAuthor.trim() !== '') {
            setOptions([...authorOptions, { value: newAuthor, label: newAuthor }]);
            setNewAuthor('');
            setIsAddingNewAuthor(false);
        }
    };

    const handleSubmit = (e) => {
        e.preventDefault();
        const blog = { title, body, author, language, like, image, comments, time };
        setIsPending(true);

        fetch('http://localhost:8000/blogs', {
            method: 'POST',
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(blog)
        }).then(() => {
            console.log("new blog added !!!" + blog.time);
            setIsPending(false);
            history.push('/');// history.go(-1);
        })
    };

    return (
        <div className="create">
            <h2>Add a New Blog</h2>
            <form onSubmit={handleSubmit}>
                <label>Blog Title</label>
                <input
                    type='text'
                    required
                    value={title}
                    onChange={(e) => setTitle(e.target.value)}
                />
                <label>Blog Language</label>
                <select
                    value={language}
                    onChange={(e) => setLanguage(e.target.value)}>
                    {languageOptions.map(option => (
                        <option key={option.value} value={option.value}>
                            {option.label}
                        </option>
                    ))}
                </select>
                <label>Blog Body</label>
                <textarea
                    required
                    value={body}
                    onChange={(e) => setBody(e.target.value)}>
                </textarea>
                <label>Blog Author</label>
                <div>
                    <select
                        value={author}
                        onChange={(e) => setAuthor(e.target.value)}>
                        {authorOptions.map(option => (
                            <option key={option.value} value={option.value}>
                                {option.label}
                            </option>
                        ))}
                    </select>
                </div>
                <div>
                    {isAddingNewAuthor ? (
                        <div>
                            <label>Add a new author:</label>
                            <input
                                type="text"
                                value={newAuthor}
                                onChange={(e) => setNewAuthor(e.target.value)}
                            />
                            <button type="button" onClick={handleAddAuthor}>
                                Add Author
                            </button>
                        </div>
                    ) : (
                        <button
                            type="button"
                            onClick={() => setIsAddingNewAuthor(true)}>
                            Add New Author
                        </button>
                    )}
                </div>
                <label>Time</label>
                <input disabled
                    type='text'
                    required
                    value={time}
                    onChange={(e) => setTitle(e.target.value)} />
                <br />
                <label>Image</label>
                <div className="image-selection">
                    {predefinedImages.map((imagePath) => (
                        <img
                            key={imagePath}
                            src={process.env.PUBLIC_URL + '/' + imagePath}
                            alt="Image Option"
                            className={`thumbnail-image ${chosenImage === imagePath ? 'selected-image' : ''}`}
                            onClick={() => handleImageClick(imagePath)}
                        />
                    ))}
                </div>
                {!isPanding && <button>Add Blog</button>}
                {isPanding && <button disabled>Adding...</button>}
            </form>
        </div>
    );
}

export default Create;