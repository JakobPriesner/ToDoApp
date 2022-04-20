import { Component, Input, Output, EventEmitter } from "@angular/core";

@Component({
    selector: 'createButton',
    templateUrl: './createButton.component.html',
    styleUrls: ['./createButton.component.scss'],
})
export class CreateButtonComponent {
    @Input() value: string;
    @Output() newClickEvent = new EventEmitter<any>();

    clickEvent(): void {
        this.newClickEvent.emit();
    }
}
